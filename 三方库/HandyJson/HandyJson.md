## HandyJSON源码解析



#### 类和结构体之间的关系

----

- protocol HandyJSON: _ExtendCustomModelType {}
- protocol _ExtendCustomModelType: _Transformable
- protocol _Transformable: _Measurable
- protocol _Measurable



- protocol PointerType : Equatable
- protocol MetadataType : PointerType
- protocol ContextDescriptorType : MetadataType
- struct Struct : ContextDescriptorType





- struct Property
- struct PropertyInfo