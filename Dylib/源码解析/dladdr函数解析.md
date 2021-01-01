## dladdr函数解析

```c++
int dladdr(const void* address, Dl_info* info)
{
	if ( dyld::gLogAPIs )
		dyld::log("%s(%p, %p)\n", __func__, address, info);

	// <rdar://problem/42171466> calling dladdr(xx,NULL) crashes
	if ( info == NULL )
		return 0; // failure

	address = stripPointer(address);

	CRSetCrashLogMessage("dyld: in dladdr()");
#if SUPPORT_ACCELERATE_TABLES
	if ( dyld::dladdrFromCache(address, info) ) {
		CRSetCrashLogMessage(NULL);
		return 1; // success
	}
#endif

	ImageLoader* image = dyld::findImageContainingAddress(address);
	if ( image != NULL ) {
		info->dli_fname = image->getRealPath();
		info->dli_fbase = (void*)image->machHeader();
		if ( address == info->dli_fbase ) {
			// special case lookup of header
			info->dli_sname = "__dso_handle";
			info->dli_saddr = info->dli_fbase;
			CRSetCrashLogMessage(NULL);
			return 1; // success
		}
		// find closest symbol in the image
		info->dli_sname = image->findClosestSymbol(address, (const void**)&info->dli_saddr);
		// never return the mach_header symbol
		if ( info->dli_saddr == info->dli_fbase ) {
			info->dli_sname = NULL;
			info->dli_saddr = NULL;
			CRSetCrashLogMessage(NULL);
			return 1; // success
		}
		if ( info->dli_sname != NULL ) {
			if ( info->dli_sname[0] == '_' )
				info->dli_sname = info->dli_sname +1; // strip off leading underscore
			//dyld::log("dladdr(%p) => %p %s\n", address, info->dli_saddr, info->dli_sname);
			CRSetCrashLogMessage(NULL);
			return 1; // success
		}
		info->dli_sname = NULL;
		info->dli_saddr = NULL;
		CRSetCrashLogMessage(NULL);
		return 1; // success
	}
	CRSetCrashLogMessage(NULL);
	return 0;  // failure
}


char* dlerror()
{
	if ( dyld::gLogAPIs )
		dyld::log("%s()\n", __func__);

	if ( dyld::gLibSystemHelpers != NULL ) {
		// if using newer libdyld.dylib and buffer if buffer not yet allocated, return NULL
		if ( dyld::gLibSystemHelpers->version >= 10 ) {
			if ( ! (*dyld::gLibSystemHelpers->hasPerThreadBufferFor_dlerror)() )
				return NULL;
		}

		// first char of buffer is flag whether string (starting at second char) is valid
		char* buffer = (*dyld::gLibSystemHelpers->getThreadBufferFor_dlerror)(2);
		if ( buffer[0] != '\0' ) {	// if valid buffer
			buffer[0] = '\0';		// mark invalid, so next call to dlerror returns NULL
			return &buffer[1];		// return message
		}
	}
	return NULL;
}
```

