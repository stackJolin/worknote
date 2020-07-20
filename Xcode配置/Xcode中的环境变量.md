## Xcode当中的环境变量



#### $SYSTEM_LIBRARY_DIR

----

系统库路径，默认为：/System/Library



#### $SYSTEM_LIBRARY_DIR/Extensions

------

系统核心扩展工程目录，里面放的都是.kext/.plugin/.bundle 等类型的文件



#### $(BUILT_PRODUCTS_DIR)

-----

build成功后的，最终产品路径－－可以在Build Settings参数的Build Location Build Products Path项里设置



####$(TARGET_NAME)

---

目标工程名称



####$(SRCROOT)

-----

工程根目录（一般为.xcodeproj所在目录）



#### $(PROJECT_DIR)

---

.xcodeproj所在目录



#### $(CURRENT_PROJECT_VERSION)

-----

$(CURRENT_PROJECT_VERSION)



#### $()

----

xcode数据目录该工程根目录



#### $(SYMROOT)

----

 $()/Build/Products



#### $(BUILD_DIR)

----

/Build/Products



#### $(BUILD_ROOT)

-----

$()/Build/Products





#### 相关文章

-----

- https://www.jianshu.com/p/74b2a1a46179
- https://www.jianshu.com/p/9145ad8180fa