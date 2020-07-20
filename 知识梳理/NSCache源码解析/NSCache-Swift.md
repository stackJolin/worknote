## NSCache.swift源码解析

源码链接:https://github.com/apple/swift-corelibs-foundation/blob/master/Sources/Foundation/NSCache.swift

```
private class NSCacheEntry<KeyType : AnyObject, ObjectType : AnyObject> {
    var key: KeyType
    var value: ObjectType
    var cost: Int
    var prevByCost: NSCacheEntry?
    var nextByCost: NSCacheEntry?
    init(key: KeyType, value: ObjectType, cost: Int) {
        self.key = key
        self.value = value
        self.cost = cost
    }
}

fileprivate class NSCacheKey: NSObject {
    
    var value: AnyObject
    
    init(_ value: AnyObject) {
        self.value = value
        super.init()
    }
    
    override var hash: Int {
        switch self.value {
        case let nsObject as NSObject:
            return nsObject.hashValue
        case let hashable as AnyHashable:
            return hashable.hashValue
        default: return 0
        }
    }
    
    override func isEqual(_ object: Any?) -> Bool {
        guard let other = (object as? NSCacheKey) else { return false }
        
        if self.value === other.value {
            return true
        } else {
            guard let left = self.value as? NSObject,
                let right = other.value as? NSObject else { return false }
            
            return left.isEqual(right)
        }
    }
}

open class NSCache<KeyType : AnyObject, ObjectType : AnyObject> : NSObject {
    
    // 保存了所有的缓存
    private var _entries = Dictionary<NSCacheKey, NSCacheEntry<KeyType, ObjectType>>()
    private let _lock = NSLock()
    private var _totalCost = 0
    
    // 头指针
    private var _head: NSCacheEntry<KeyType, ObjectType>?
    
    open var name: String = ""
    open var totalCostLimit: Int = 0 // limits are imprecise/not strict
    open var countLimit: Int = 0 // limits are imprecise/not strict
    open var evictsObjectsWithDiscardedContent: Bool = false
    
    public override init() {}
    
    open weak var delegate: NSCacheDelegate?
    
    // 对外方法
    open func object(forKey key: KeyType) -> ObjectType? {
        var object: ObjectType?
        
        let key = NSCacheKey(key)
        
        _lock.lock()
        if let entry = _entries[key] {
            object = entry.value
        }
        _lock.unlock()
        
        return object
    }
    
    // 对方方法，cost永远是0。也就是totalCostLimit并没有什么作用
    open func setObject(_ obj: ObjectType, forKey key: KeyType) {
        setObject(obj, forKey: key, cost: 0)
    }
    
    // 删除entity
    private func remove(_ entry: NSCacheEntry<KeyType, ObjectType>) {
        let oldPrev = entry.prevByCost
        let oldNext = entry.nextByCost
        
        oldPrev?.nextByCost = oldNext
        oldNext?.prevByCost = oldPrev
        
        if entry === _head {
            _head = oldNext
        }
    }
   
    // 插入entity，并且，更新entity的pre和next的指针
    private func insert(_ entry: NSCacheEntry<KeyType, ObjectType>) {
    
        // 如果不存在头部元素，说明插入的是第一个元素
        guard var currentElement = _head else {
            // The cache is empty
            entry.prevByCost = nil
            entry.nextByCost = nil
            
            _head = entry
            return
        }
        
        // 如果头指针存在，对比插入的实体的cost和head的cost
        guard entry.cost > currentElement.cost else {
            // Insert entry at the head
            entry.prevByCost = nil
            entry.nextByCost = currentElement
            currentElement.prevByCost = entry
            
            _head = entry
            return
        }
        
        // cost越大的，越放在后面
        while let nextByCost = currentElement.nextByCost, nextByCost.cost < entry.cost {
            currentElement = nextByCost
        }
        
        // Insert entry between currentElement and nextElement
        let nextElement = currentElement.nextByCost
        
        currentElement.nextByCost = entry
        entry.prevByCost = currentElement
        
        entry.nextByCost = nextElement
        nextElement?.prevByCost = entry
    }
    
    open func setObject(_ obj: ObjectType, forKey key: KeyType, cost g: Int) {
        let g = max(g, 0)
        let keyRef = NSCacheKey(key)
        
        _lock.lock()
        
        let costDiff: Int
        
        // 如果key对应的entity存在
        if let entry = _entries[keyRef] {
            costDiff = g - entry.cost
            entry.cost = g
            
            entry.value = obj
            
            if costDiff != 0 {
                remove(entry)
                insert(entry)
            }
        } 
        else { // 如果key对应的entity不存在，创建entity并插入
            let entry = NSCacheEntry(key: key, value: obj, cost: g)
            _entries[keyRef] = entry
            insert(entry)
            
            costDiff = g
        }
        
        _totalCost += costDiff
        
        // 计算应该被清除的“价值”（因为缓存容量有限，现在要添加新的进来，很可能超过限制，需要移除一些旧的）
        var purgeAmount = (totalCostLimit > 0) ? (_totalCost - totalCostLimit) : 0
        while purgeAmount > 0 {
            if let entry = _head {
                delegate?.cache(unsafeDowncast(self, to:NSCache<AnyObject, AnyObject>.self), willEvictObject: entry.value)
                
                _totalCost -= entry.cost
                purgeAmount -= entry.cost
                
                // 移除顶部元素
                remove(entry) // _head will be changed to next entry in remove(_:)
                _entries[NSCacheKey(entry.key)] = nil
            } 
            else {
                break
            }
        }
        
        var purgeCount = (countLimit > 0) ? (_entries.count - countLimit) : 0
        while purgeCount > 0 {
            if let entry = _head {
                delegate?.cache(unsafeDowncast(self, to:NSCache<AnyObject, AnyObject>.self), willEvictObject: entry.value)
                
                _totalCost -= entry.cost
                purgeCount -= 1
                
                // 移除顶部元素
                remove(entry) // _head will be changed to next entry in remove(_:)
                _entries[NSCacheKey(entry.key)] = nil
            } else {
                break
            }
        }
        
        _lock.unlock()
    }
    
    open func removeObject(forKey key: KeyType) {
        let keyRef = NSCacheKey(key)
        
        _lock.lock()
        if let entry = _entries.removeValue(forKey: keyRef) {
            _totalCost -= entry.cost
            remove(entry)
        }
        _lock.unlock()
    }
    
    open func removeAllObjects() {
        _lock.lock()
        _entries.removeAll()
        
        while let currentElement = _head {
            let nextElement = currentElement.nextByCost
            
            currentElement.prevByCost = nil
            currentElement.nextByCost = nil
            
            _head = nextElement
        }
        
        _totalCost = 0
        _lock.unlock()
    }    
}

public protocol NSCacheDelegate : NSObjectProtocol {
    func cache(_ cache: NSCache<AnyObject, AnyObject>, willEvictObject obj: Any)
}

extension NSCacheDelegate {
    func cache(_ cache: NSCache<AnyObject, AnyObject>, willEvictObject obj: Any) {
        // Default implementation does nothing
    }
}
```





#### NSCache-Swift实现思路

-----

