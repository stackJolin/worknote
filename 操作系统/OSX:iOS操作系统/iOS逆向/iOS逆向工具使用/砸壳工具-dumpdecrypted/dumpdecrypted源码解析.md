## dumpdecrypted

源码：

- 原始版本：https://github.com/stefanesser/dumpdecrypted
- 改进版本：https://github.com/conradev/dumpdecrypted/blob/master/dumpdecrypted.c(本文使用的版本)



#### dumpdecrypted的工作过程

-------

- 在`main`函数执行之前，`+load`方法执行之后，执行方法`dumpexecutable`，注册、监听动态库/bundle的加载
- 当动态库/bundle加载后，回调给`dumpdecrypted`注册的函数`image_added`，获取动态库/bundle(就是Mach-O的文件)的`head`指针
- 将上面获得的`head`指针传递给`dumptofile`函数，执行真正文件到处工作

#### 原理

-----

dumpdecrypted属于`动态砸壳`一类。

dumpdecrypted只有一个文件，一个函数，源码如下：

```c
/*
Dumps decrypted iPhone Applications to a file - better solution than those GDB scripts for non working GDB versions
(C) Copyright 2011-2014 Stefan Esser
iPod:~ root# DYLD_INSERT_LIBRARIES=dumpdecrypted.dylib /var/mobile/Applications/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx/Scan.app/Scan
mach-o decryption dumper
DISCLAIMER: This tool is only meant for security research purposes, not for application crackers.
[+] Found encrypted data at address 00002000 of length 1826816 bytes - type 1.
[+] Opening /private/var/mobile/Applications/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx/Scan.app/Scan for reading.
[+] Reading header
[+] Detecting header type
[+] Executable is a FAT image - searching for right architecture
[+] Correct arch is at offset 2408224 in the file
[+] Opening Scan.decrypted for writing.
[-] Failed opening. Most probably a sandbox issue. Trying something different.
[+] Opening /private/var/mobile/Applications/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx/tmp/Scan.decrypted for writing.
[+] Copying the not encrypted start of the file
[+] Dumping the decrypted data into the file
[+] Copying the not encrypted remainder of the file
[+] Closing original file
[+] Closing dump file
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <mach-o/fat.h>
#include <mach-o/loader.h>
#include <mach-o/dyld.h>

// 定义一个宏，把数字从小端序转成大端序
#define swap32(value) (((value & 0xFF000000) >> 24) | 
                        ((value & 0x00FF0000) >> 8) | 
                        ((value & 0x0000FF00) << 8) | 
                        ((value & 0x000000FF) << 24) )

void dumptofile(const char *path, const struct mach_header *mh) {
	struct load_command *lc;
	struct encryption_info_command *eic;
	struct fat_header *fh;
	struct fat_arch *arch;
	char buffer[1024];
	char rpath[4096],npath[4096]; /* should be big enough for PATH_MAX */
	unsigned int fileoffs = 0, off_cryptid = 0, restsize;
	int i,fd,outfd,r,n,toread;
	char *tmp;
  // realpath:将相对路径转换为绝对路径
	if (realpath(path, rpath) == NULL) { // 如果失败，直接copy路径赋值给rpath
		strlcpy(rpath, path, sizeof(rpath));
	}

	/* extract basename */
  // 最后出现`/`的位置
	tmp = strrchr(rpath, '/');
	printf("\n\n");
	if (tmp == NULL) {
		printf("[-] Unexpected error with filename.\n");
		_exit(1);
	} else {
		printf("[+] Dumping %s\n", tmp+1);
	}

	/* detect if this is a arm64 binary */
	if (mh->magic == MH_MAGIC_64) { // 如果是CPU架构是64位
    // 根据`mach_header`长度，计算第一个`load_command`的地址
		lc = (struct load_command *)((unsigned char *)mh + sizeof(struct mach_header_64));
		printf("[+] detected 64bit ARM binary in memory.\n");
	} 
  else { /* we might want to check for other errors here, too */
    // 根据`mach_header`长度，计算第一个`load_command`的地址
		lc = (struct load_command *)((unsigned char *)mh + sizeof(struct mach_header));
		printf("[+] detected 32bit ARM binary in memory.\n");
	}
	
	/* searching all load commands for an LC_ENCRYPTION_INFO load command */
	for (i=0; i<mh->ncmds; i++) {
		/*printf("Load Command (%d): %08x\n", i, lc->cmd);*/
		
    // 这个if语句是重中之重
		if (lc->cmd == LC_ENCRYPTION_INFO || lc->cmd == LC_ENCRYPTION_INFO_64) {
			eic = (struct encryption_info_command *)lc;
			
			/* If this load command is present, but data is not crypted then exit */
      /* 如果没有加密，直接返回*/
			if (eic->cryptid == 0) {
				break;
			}
      
      // load_command加密标识偏移
			off_cryptid=(off_t)((void*)&eic->cryptid - (void*)mh);
			printf("[+] offset to cryptid found: @%p(from %p) = %x\n", &eic->cryptid, mh, off_cryptid);

			printf("[+] Found encrypted data at address %08x of length %u bytes - type %u.\n", eic->cryptoff, eic->cryptsize, eic->cryptid);

			printf("[+] Opening %s for reading.\n", rpath);
      // 打开文件，拿到句柄(其实是将文件读取到内存)
			fd = open(rpath, O_RDONLY);
			if (fd == -1) { // 如果打开失败、直接返回
				printf("[-] Failed opening.\n");
				_exit(1);
			}
			
			printf("[+] Reading header\n");
      // 读取1024个字节到buffer中
			n = read(fd, (void *)buffer, sizeof(buffer));
			if (n != sizeof(buffer)) {
				printf("[W] Warning read only %d bytes\n", n);
			}
			
			printf("[+] Detecting header type\n");
      // 为什么1024个字节就是一个`header`
			fh = (struct fat_header *)buffer;
			
			/* Is this a FAT file - we assume the right endianess */
			if (fh->magic == FAT_CIGAM) { // 如果是胖二进制
				printf("[+] Executable is a FAT image - searching for right architecture\n");
				arch = (struct fat_arch *)&fh[1];
        // 查找当前CPU架构的二进制文件在内存中的偏移
				for (i=0; i<swap32(fh->nfat_arch); i++) {
					if ((mh->cputype == swap32(arch->cputype)) && (mh->cpusubtype == swap32(arch->cpusubtype))) {
						fileoffs = swap32(arch->offset);
						printf("[+] Correct arch is at offset %u in the file\n", fileoffs);
						break;
					}
					arch++;
				}
				if (fileoffs == 0) {
					printf("[-] Could not find correct arch in FAT image\n");
					_exit(1);
				}
			} 
      else if (fh->magic == MH_MAGIC || fh->magic == MH_MAGIC_64) { // 如果是纯mach-o文件
				printf("[+] Executable is a plain MACH-O image\n");
			} 
      else { // 如果文件类型
				printf("[-] Executable is of unknown type\n");
				_exit(1);
			}
      // 将 tmp+1指针地址开始的字符串，复制到npath地址开始的指针地址下-其实就是在复制文件名
			strlcpy(npath, tmp+1, sizeof(npath));
      // 文件名后面拼接`.decrypted`，比如xx.decrypted
			strlcat(npath, ".decrypted", sizeof(npath));
      // 把这个名字复制到`buffer`指针下。buffer下存的是`xx.decrypted`字符串
			strlcpy(buffer, npath, sizeof(buffer));

			printf("[+] Opening %s for writing.\n", npath);
      
      // 创建一个输出文件，并获取句柄
			outfd = open(npath, O_RDWR|O_CREAT|O_TRUNC, 0644);
      
			if (outfd == -1) { // 如果创建失败
				if (strncmp("/private/var/mobile/Applications/", rpath, 33) == 0) {
					printf("[-] Failed opening. Most probably a sandbox issue. Trying something different.\n");
					
					/* create new name */
					strlcpy(npath, "/private/var/mobile/Applications/", sizeof(npath));
					tmp = strchr(rpath+33, '/');
					if (tmp == NULL) {
						printf("[-] Unexpected error with filename.\n");
						_exit(1);
					}
					tmp++;
					*tmp++ = 0;
					strlcat(npath, rpath+33, sizeof(npath));
					strlcat(npath, "tmp/", sizeof(npath));
					strlcat(npath, buffer, sizeof(npath));
					printf("[+] Opening %s for writing.\n", npath);
					outfd = open(npath, O_RDWR|O_CREAT|O_TRUNC, 0644);
				}
				if (outfd == -1) {
					perror("[-] Failed opening");
					printf("\n");
					_exit(1);
				}
			}
			
			/* calculate address of beginning of crypted data */
      // 加密数据部分之前的数据大小
			n = fileoffs + eic->cryptoff;
			
			restsize = lseek(fd, 0, SEEK_END) - n - eic->cryptsize;			
			lseek(fd, 0, SEEK_SET);
			
			printf("[+] Copying the not encrypted start of the file\n");
			/* first copy all the data before the encrypted data */
      /* 首先，copy所有加密数据之前的数据 */
			while (n > 0) {
				toread = (n > sizeof(buffer)) ? sizeof(buffer) : n;
				r = read(fd, buffer, toread);
				if (r != toread) {
					printf("[-] Error reading file\n");
					_exit(1);
				}
				n -= r;
				
				r = write(outfd, buffer, toread);
				if (r != toread) {
					printf("[-] Error writing file\n");
					_exit(1);
				}
			}
			
			/* now write the previously encrypted data */
      /* 然后，写入加密部分的数据(也就是砸壳之后的数据) */
			printf("[+] Dumping the decrypted data into the file\n");
			r = write(outfd, (unsigned char *)mh + eic->cryptoff, eic->cryptsize);
			if (r != eic->cryptsize) {
				printf("[-] Error writing file\n");
				_exit(1);
			}
			
			/* and finish with the remainder of the file */
      /* 写入剩余的数据 */
			n = restsize;
			lseek(fd, eic->cryptsize, SEEK_CUR);
			printf("[+] Copying the not encrypted remainder of the file\n");
			while (n > 0) {
				toread = (n > sizeof(buffer)) ? sizeof(buffer) : n;
				r = read(fd, buffer, toread);
				if (r != toread) {
					printf("[-] Error reading file\n");
					_exit(1);
				}
				n -= r;
				
				r = write(outfd, buffer, toread);
				if (r != toread) {
					printf("[-] Error writing file\n");
					_exit(1);
				}
			}

			if (off_cryptid) { // 如果加密部分到首页的偏移大于0
				uint32_t zero=0;
        // 加密部分的偏移应该加上fileoffs，fileoffs只在胖二进制的情况下出现
				off_cryptid+=fileoffs;
				printf("[+] Setting the LC_ENCRYPTION_INFO->cryptid to 0 at offset %x\n", off_cryptid);
				if (lseek(outfd, off_cryptid, SEEK_SET) != off_cryptid || write(outfd, &zero, 4) != 4) {
					printf("[-] Error writing cryptid value\n");
				}
			}

			printf("[+] Closing original file\n");
			close(fd);
			printf("[+] Closing dump file\n");
			close(outfd);
		}
		
		lc = (struct load_command *)((unsigned char *)lc+lc->cmdsize);		
	}
	printf("[-] This mach-o file is not encrypted. Nothing was decrypted.\n");
}

static void image_added(const struct mach_header *mh, intptr_t slide) {
	Dl_info image_info;
	int result = dladdr(mh, &image_info);
	dumptofile(image_info.dli_fname, mh);
}

__attribute__((constructor))
static void dumpexecutable() {
	printf("mach-o decryption dumper\n\n");
	printf("DISCLAIMER: This tool is only meant for security research purposes, not for application crackers.");
	_dyld_register_func_for_add_image(&image_added);
}
```

<font color='orange'>`__attribute__((constructor))`:</font>

`__attribute__`是一个编译属性，用于向编译器描述特殊的标识、错误检查或高级优化。它是GNU C特色之一，系统中有许多地方使用到。 `__attribute__`可以设置函数属性（Function Attribute ）、变量属性（Variable Attribute ）和类型属性（Type Attribute)等。`constructor`表示在`main`之前，`+()load`之后执行

<font color='orange'>`_dyld_register_func_for_add_image`:</font>

位于属于`dyld`源码。

```c++
/*
 * _dyld_register_func_for_add_image registers the specified function to be
 * called when a new image is added (a bundle or a dynamic shared library) to
 * the program.  When this function is first registered it is called for once
 * for each image that is currently part of the program.
 */
/*
 * 注册一个监听，监听动态库/Bundle的加载进程序，每个库只会调用一次
**/
void
_dyld_register_func_for_add_image(
void (*func)(const struct mach_header *mh, intptr_t vmaddr_slide))
```

<font color='orange'>`lseek`:</font>

C函数，定义函数：`off_t lseek(int fildes, off_t offset, int whence);`

函数说明：

```markdown
每一个已打开的文件都有一个读写位置, 当打开文件时通常其读写位置是指向文件开头, 若是以附加的方式打开文件(如O_APPEND), 则读写位置会指向文件尾. 当read()或write()时, 读写位置会随之增加,lseek()便是用来控制该文件的读写位置. 参数fildes 为已打开的文件描述词, 参数offset 为根据参数whence来移动读写位置的位移数.
参数 whence 为下列其中一种:
    SEEK_SET 参数offset 即为新的读写位置.
    SEEK_CUR 以目前的读写位置往后增加offset 个位移量.
    SEEK_END 将读写位置指向文件尾后再增加offset 个位移量. 当whence 值为SEEK_CUR 或
    SEEK_END 时, 参数offet 允许负值的出现.
```

<font color='orange'>`strlcpy`:</font>

```markdown
函数原型声明：size_t strlcpy(char *dest, const char *src, size_t size)
头文件引用：#include <string.h>
功能： 在已知dest缓冲区大小并不会造成[缓冲区溢出](https://baike.baidu.com/item/缓冲区溢出)前提下，将src地址开始的字符串复制到以dest开始的[地址空间](https://baike.baidu.com/item/地址空间)
返回值：src字符串的大小
函数参数：参数dest为目的字符串开始的指针，src为源字符串的开始地址，参数size代表dest字符串的大小
```



<font color='orange'>`Dl_info`:</font>

```c++
typedef struct dl_info {
    const char  *dli_fname;     /* Pathname of shared object */
    void        *dli_fbase;     /* Base address of shared object */
    const char  *dli_sname;     /* Name of nearest symbol */
    void        *dli_saddr;     /* Address of nearest symbol */
} Dl_info;
```

- dli_fname：路径名
- dli_fbase：镜像的起始地址
- dli_sname：符号名字
- dli_saddr：符号地址



<font color='orange'>`dladdr`:</font>

Mach-O的头部的大部分信息都可以通过定义在dlfcn.h中的函数`dladdr`来获取到



#### Makefile

-----

我们再来看一下`Makefile`的构造：

```makefile
GCC_BIN=`xcrun --sdk iphoneos --find gcc`
GCC_UNIVERSAL=$(GCC_BASE) -arch armv7 -arch armv7s -arch arm64
SDK=`xcrun --sdk iphoneos --show-sdk-path`

# 表示用于C编译器的选项，可以为空
CFLAGS = 
GCC_BASE = $(GCC_BIN) -Os $(CFLAGS) -Wimplicit -isysroot $(SDK) -F$(SDK)/System/Library/Frameworks -F$(SDK)/System/Library/PrivateFrameworks

all: dumpdecrypted.dylib

# $@：当前的构建目标，即 dumpdecrypted.dylib
# #^：指代所有的前置条件，之间以空格分开，这里只有一个前置条件，即 dumpdecrypted.o 
dumpdecrypted.dylib: dumpdecrypted.o 
	$(GCC_UNIVERSAL) -dynamiclib -o $@ $^
	
# dumpdecrypted.o:dumpdecrypted.c
# $<：指代第一个前置条件，即 dumpdecrypted.c
%.o: %.c
	$(GCC_UNIVERSAL) -c -o $@ $< 

clean:
	rm -f *.o dumpdecrypted.dylib
```

关于`makefile`的一些语法，详见：https://www.ruanyifeng.com/blog/2015/02/make.html



#### 相关文章

https://feicong.github.io/2017/01/13/macho/