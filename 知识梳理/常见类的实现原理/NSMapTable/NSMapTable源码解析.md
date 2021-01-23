## NSMapTable源码解析

-------

NSMaptable.h - https://github.com/gnustep/libs-base/blob/master/Headers/Foundation/NSMapTable.h

NSMaptable.m - https://github.com/gnustep/libs-base/blob/master/Source/NSMapTable.m

NSConcreteMapTable.m - https://github.com/gnustep/libs-base/blob/master/Source/NSConcreteMapTable.m



#### NSMaptable.h

------

```objective-c
/* NSMapTable interface for GNUStep.
 * Copyright (C) 1994, 1995, 1996, 2002, 2009  Free Software Foundation, Inc.
 * 
 * Author: Albin L. Jones <Albin.L.Jones@Dartmouth.EDU>
 * Created: Tue Dec 13 00:05:02 EST 1994
 * Updated: Thu Mar 21 15:12:42 EST 1996
 * Serial: 96.03.21.05
 * Modified by: Richard Frith-Macdonald <rfm@gnu.org>
 * Updated: March 2009
 * 
 * This file is part of the GNUstep Base Library.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110 USA.
 */ 

#ifndef __NSMapTable_h_GNUSTEP_BASE_INCLUDE
#define __NSMapTable_h_GNUSTEP_BASE_INCLUDE 1
#import	<GNUstepBase/GSVersionMacros.h>

/**** Included Headers *******************************************************/

#import	<Foundation/NSObject.h>
#import	<Foundation/NSString.h>
#import	<Foundation/NSArray.h>
#import	<Foundation/NSEnumerator.h>
#import	<Foundation/NSPointerFunctions.h>

#if	defined(__cplusplus)
extern "C" {
#endif

/**** Type, Constant, and Macro Definitions **********************************/

enum {
  NSMapTableStrongMemory
    = NSPointerFunctionsStrongMemory,
  NSMapTableZeroingWeakMemory
    = NSPointerFunctionsZeroingWeakMemory,
  NSMapTableCopyIn
    = NSPointerFunctionsCopyIn,
  NSMapTableObjectPointerPersonality
    = NSPointerFunctionsObjectPointerPersonality,
  NSMapTableWeakMemory
    = NSPointerFunctionsWeakMemory
};

typedef NSUInteger NSMapTableOptions;

@interface GS_GENERIC_CLASS(NSMapTable, KeyT, ValT)
  : NSObject <NSCopying, NSCoding, NSFastEnumeration>

/** Return a map table initialised using the specified options for
 * keys and values.
 */
+ (instancetype) mapTableWithKeyOptions: (NSPointerFunctionsOptions)keyOptions
                           valueOptions: (NSPointerFunctionsOptions)valueOptions;

/** Convenience method for creating a map table to store object values
 * using object keys.
 */
// 便利构造器 - `key`是强引用，而`value`也是强引用
+ (instancetype) mapTableWithStrongToStrongObjects;

/** Convenience method for creating a map table to store non-retained
 * object values with retained object keys.
 */
// 便利构造器 - `key`是强引用，而`value`是弱引用
+ (instancetype) mapTableWithStrongToWeakObjects;

/** Convenience method for creating a map table to store retained
 * object values with non-retained object keys.
 */
// 便利构造器 - `key`是弱引用，而`value`是强引用
+ (instancetype) mapTableWithWeakToStrongObjects;

/** Convenience method for creating a map table to store non-retained
 * object values with non-retained object keys.
 */
// 便利构造器 - `key`是强引用，`value`是也是弱引用
+ (instancetype) mapTableWithWeakToWeakObjects;

/** Convenience method for creating a map table to store object values
 * using object keys.  The collection will retain both the key and the value.
 */
 // 便利构造器 - `key`是强引用，`value`是也是强引用
+ (instancetype) strongToStrongObjectsMapTable;
/** Convenience method for creating a map table to store object values
 * using object keys.  The collection will retain the key, the value will be a
 * zeroing weak reference.
 */
// 便利构造器 - `key`是强引用，而`value`是弱引用
+ (instancetype) strongToWeakObjectsMapTable;
/** Convenience method for creating a map table to store object values
 * using object keys.  The collection will retain the value, the key will be a
 * zeroing weak reference.
 */
// 便利构造器 - `key`是弱引用，而`value`是强引用
+ (instancetype) weakToStrongObjectsMapTable;
/** Convenience method for creating a map table to store object values
 * using object keys.  The collection will use zeroing weak references for both
 * the key and the value.
 */
// 便利构造器 - `key`是弱引用，而`value`也是弱引用
+ (instancetype) weakToWeakObjectsMapTable;


/** Initialiser using option bitmasks to describe the keys and values.
 */
- (instancetype) initWithKeyOptions: (NSPointerFunctionsOptions)keyOptions
                       valueOptions: (NSPointerFunctionsOptions)valueOptions
                           capacity: (NSUInteger)initialCapacity;

/** Initialiser using full pointer function information to describe
 * the keys and values.
 */
- (instancetype) initWithKeyPointerFunctions: (NSPointerFunctions*)keyFunctions
                       valuePointerFunctions: (NSPointerFunctions*)valueFunctions
                                    capacity: (NSUInteger)initialCapacity;

/** Return the number of items stored in the map.
 */
// NSMaptable存储的value数量
- (NSUInteger) count;

/** Return a dictionary containing the keys and values in the receiver.
 */
// NSMapTable转NSDictionary
- (NSDictionary*) dictionaryRepresentation;

/** Return an enumerator able to enumerate the keys in the receiver.
 */
// key的迭代器
- (GS_GENERIC_CLASS(NSEnumerator, KeyT)*) keyEnumerator;

/** Return an NSPointerFunctions value describind the functions used by the
 * receiver to handle keys.
 */
- (NSPointerFunctions*) keyPointerFunctions;

/** Return an enumerator able to enumerate the values in the receiver.
 */
// value的迭代器
- (GS_GENERIC_CLASS(NSEnumerator, ValT)*) objectEnumerator;

/** Return the object stored under the specified key.
 */
- (GS_GENERIC_TYPE(ValT)) objectForKey: (GS_GENERIC_TYPE(KeyT))aKey;

/** Empty the receiver of all stored values.
 */
- (void) removeAllObjects;

/** Remove the object stored under the specified key.
 */
- (void) removeObjectForKey: (GS_GENERIC_TYPE(KeyT))aKey;

/** Store the object under the specified key, replacing any object which
 * was previously stored under that key.
 */
- (void) setObject: (GS_GENERIC_TYPE(ValT))anObject forKey: (GS_GENERIC_TYPE(KeyT))aKey;

/** Return an NSPointerFunctions value describind the functions used by the
 * receiver to handle values.
 */
- (NSPointerFunctions*) valuePointerFunctions;
@end

/**
 * Type for enumerating.<br />
 * NB. Implementation detail ... in GNUstep the layout <strong>must</strong>
 * correspond to that used by the GSIMap macros.
 */
typedef struct { void *map; void *node; size_t bucket; } NSMapEnumerator;

/**
 * Callback functions for a key.
 */
typedef struct _NSMapTableKeyCallBacks
{
  /*
   * Hashing function. Must not modify the key.<br />
   * NOTE: Elements with equal values must
   * have equal hash function values.
   */
  NSUInteger (*hash)(NSMapTable *, const void *);

  /**
   * Comparison function.  Must not modify either key.
   */
  BOOL (*isEqual)(NSMapTable *, const void *, const void *);

  /**
   * Retaining function called when adding elements to table.<br />
   * Notionally this must not modify the key (the key may not
   * actually have a retain count, or the retain count may be stored
   * externally to the key, but in practice this often actually
   * changes a counter within the key).
   */
  void (*retain)(NSMapTable *, const void *);

  /**
   * Releasing function called when a data element is
   * removed from the table.  This may decrease a retain count or may
   * actually destroy the key.
   */
  void (*release)(NSMapTable *, void *);

  /**
   * Description function. Generates a string describing the key
   * and does not modify the key itself.
   */ 
  NSString *(*describe)(NSMapTable *, const void *);

  /**
   * Quantity that is not a key to the map table.
   */
  const void *notAKeyMarker;
} NSMapTableKeyCallBacks;

/**
 * Callback functions for a value.
 */
typedef struct _NSMapTableValueCallBacks NSMapTableValueCallBacks;
struct _NSMapTableValueCallBacks
{
  /**
   * Retaining function called when adding elements to table.<br />
   * Notionally this must not modify the element (the element may not
   * actually have a retain count, or the retain count may be stored
   * externally to the element, but in practice this often actually
   * changes a counter within the element).
   */
  void (*retain)(NSMapTable *, const void *);

  /**
   * Releasing function called when a data element is
   * removed from the table.  This may decrease a retain count or may
   * actually destroy the element.
   */
  void (*release)(NSMapTable *, void *);

  /**
   * Description function. Generates a string describing the element
   * and does not modify the element itself.
   */ 
  NSString *(*describe)(NSMapTable *, const void *);
};

/* Quantities that are never map keys. */
#define NSNotAnIntMapKey     ((const void *)0x80000000)
#define NSNotAPointerMapKey  ((const void *)0xffffffff)

GS_EXPORT const NSMapTableKeyCallBacks NSIntegerMapKeyCallBacks;
GS_EXPORT const NSMapTableKeyCallBacks NSIntMapKeyCallBacks; /*DEPRECATED*/
GS_EXPORT const NSMapTableKeyCallBacks NSNonOwnedPointerMapKeyCallBacks;
GS_EXPORT const NSMapTableKeyCallBacks NSNonOwnedPointerOrNullMapKeyCallBacks;
GS_EXPORT const NSMapTableKeyCallBacks NSNonRetainedObjectMapKeyCallBacks;
GS_EXPORT const NSMapTableKeyCallBacks NSObjectMapKeyCallBacks;
GS_EXPORT const NSMapTableKeyCallBacks NSOwnedPointerMapKeyCallBacks;
GS_EXPORT const NSMapTableValueCallBacks NSIntegerMapValueCallBacks;
GS_EXPORT const NSMapTableValueCallBacks NSIntMapValueCallBacks; /*DEPRECATED*/
GS_EXPORT const NSMapTableValueCallBacks NSNonOwnedPointerMapValueCallBacks;
GS_EXPORT const NSMapTableValueCallBacks NSNonRetainedObjectMapValueCallBacks;
GS_EXPORT const NSMapTableValueCallBacks NSObjectMapValueCallBacks;
GS_EXPORT const NSMapTableValueCallBacks NSOwnedPointerMapValueCallBacks;

GS_EXPORT NSMapTable *
NSCreateMapTable(NSMapTableKeyCallBacks keyCallBacks,
                 NSMapTableValueCallBacks valueCallBacks,
                 NSUInteger capacity);

GS_EXPORT NSMapTable *
NSCreateMapTableWithZone(NSMapTableKeyCallBacks keyCallBacks,
                         NSMapTableValueCallBacks valueCallBacks,
                         NSUInteger capacity,
                         NSZone *zone);

GS_EXPORT NSMapTable *
NSCopyMapTableWithZone(NSMapTable *table, NSZone *zone);

GS_EXPORT void
NSFreeMapTable(NSMapTable *table);

GS_EXPORT void
NSResetMapTable(NSMapTable *table);

GS_EXPORT BOOL
NSCompareMapTables(NSMapTable *table1, NSMapTable *table2);

GS_EXPORT NSUInteger
NSCountMapTable(NSMapTable *table);

GS_EXPORT BOOL
NSMapMember(NSMapTable *table,
            const void *key,
            void **originalKey,
            void **value);

GS_EXPORT void *
NSMapGet(NSMapTable *table, const void *key);

GS_EXPORT void
NSEndMapTableEnumeration(NSMapEnumerator *enumerator);

GS_EXPORT NSMapEnumerator
NSEnumerateMapTable(NSMapTable *table);

GS_EXPORT BOOL
NSNextMapEnumeratorPair(NSMapEnumerator *enumerator,
                        void **key,
                        void **value);

GS_EXPORT NSArray *
NSAllMapTableKeys(NSMapTable *table);

GS_EXPORT NSArray *
NSAllMapTableValues(NSMapTable *table);

GS_EXPORT void
NSMapInsert(NSMapTable *table, const void *key, const void *value);

GS_EXPORT void *
NSMapInsertIfAbsent(NSMapTable *table, const void *key, const void *value);

GS_EXPORT void
NSMapInsertKnownAbsent(NSMapTable *table,
                       const void *key,
                       const void *value);

GS_EXPORT void
NSMapRemove(NSMapTable *table, const void *key);

GS_EXPORT NSString *NSStringFromMapTable (NSMapTable *table);

#if	defined(__cplusplus)
}
#endif

#endif /* __NSMapTable_h_GNUSTEP_BASE_INCLUDE */
```



#### NSMapTable.m

------

```objective-c
/** NSMapTable implementation for GNUStep.
 * Copyright (C) 2009  Free Software Foundation, Inc.
 *
 * Author: Richard Frith-Macdonald <rfm@gnu.org>
 *
 * This file is part of the GNUstep Base Library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110 USA.
 *
 * <title>NSMapTable class reference</title>
 * $Date$ $Revision$
 */

#import "common.h"
#import "Foundation/NSArray.h"
#import "Foundation/NSDictionary.h"
#import "Foundation/NSException.h"
#import "Foundation/NSPointerFunctions.h"
#import "Foundation/NSMapTable.h"
#import "NSCallBacks.h"

@interface	NSConcreteMapTable : NSMapTable
@end

@implementation	NSMapTable

static Class	abstractClass = 0;
static Class	concreteClass = 0;

+ (id) allocWithZone: (NSZone*)aZone
{
  if (self == abstractClass)
    {
      return NSAllocateObject(concreteClass, 0, aZone);
    }
  return NSAllocateObject(self, 0, aZone);
}

+ (void) initialize
{
  if (abstractClass == 0)
    {
      abstractClass = [NSMapTable class];
      concreteClass = [NSConcreteMapTable class];
    }
}

+ (id) mapTableWithKeyOptions: (NSPointerFunctionsOptions)keyOptions
		 valueOptions: (NSPointerFunctionsOptions)valueOptions
{
  NSMapTable	*t;

  t = [self allocWithZone: NSDefaultMallocZone()];
  t = [t initWithKeyOptions: keyOptions
	       valueOptions: valueOptions
		   capacity: 0];
  return AUTORELEASE(t);
}

+ (id) mapTableWithStrongToStrongObjects
{
  return [self mapTableWithKeyOptions: NSPointerFunctionsObjectPersonality
			 valueOptions: NSPointerFunctionsObjectPersonality];
}

+ (id) mapTableWithStrongToWeakObjects
{
  return [self mapTableWithKeyOptions: NSPointerFunctionsObjectPersonality
			 valueOptions: NSPointerFunctionsObjectPersonality
    | NSPointerFunctionsZeroingWeakMemory];
}

+ (id) mapTableWithWeakToStrongObjects
{
  return [self mapTableWithKeyOptions: NSPointerFunctionsObjectPersonality
    | NSPointerFunctionsZeroingWeakMemory
			 valueOptions: NSPointerFunctionsObjectPersonality];
}

+ (id) mapTableWithWeakToWeakObjects
{
  return [self mapTableWithKeyOptions: NSPointerFunctionsObjectPersonality
    | NSPointerFunctionsZeroingWeakMemory
			 valueOptions: NSPointerFunctionsObjectPersonality
    | NSPointerFunctionsZeroingWeakMemory];
}

+ (id) strongToStrongObjectsMapTable
{
  return [self mapTableWithKeyOptions: NSPointerFunctionsObjectPersonality
                         valueOptions: NSPointerFunctionsObjectPersonality];
}

+ (id) strongToWeakObjectsMapTable
{
  return [self mapTableWithKeyOptions: NSPointerFunctionsObjectPersonality 
                         valueOptions: NSPointerFunctionsObjectPersonality |
                                         NSMapTableWeakMemory];
}

+ (id) weakToStrongObjectsMapTable
{
  return [self mapTableWithKeyOptions: NSPointerFunctionsObjectPersonality |
                                         NSMapTableWeakMemory
                         valueOptions: NSPointerFunctionsObjectPersonality];
}

+ (id) weakToWeakObjectsMapTable
{
  return [self mapTableWithKeyOptions: NSPointerFunctionsObjectPersonality | 
                                         NSMapTableWeakMemory
                         valueOptions: NSPointerFunctionsObjectPersonality |
                                         NSMapTableWeakMemory];
}

- (id) initWithKeyOptions: (NSPointerFunctionsOptions)keyOptions
	     valueOptions: (NSPointerFunctionsOptions)valueOptions
	         capacity: (NSUInteger)initialCapacity
{
  NSPointerFunctions	*k;
  NSPointerFunctions	*v;
  id			o;

  k = [[NSPointerFunctions alloc] initWithOptions: keyOptions];
  v = [[NSPointerFunctions alloc] initWithOptions: valueOptions];
  o = [self initWithKeyPointerFunctions: k
		  valuePointerFunctions: v
			       capacity: initialCapacity];
  [k release];
  [v release];
  return o;
}

- (id) initWithKeyPointerFunctions: (NSPointerFunctions*)keyFunctions
	     valuePointerFunctions: (NSPointerFunctions*)valueFunctions
			  capacity: (NSUInteger)initialCapacity
{
  [self subclassResponsibility: _cmd];
  return nil;
}

- (id) copyWithZone: (NSZone*)aZone
{
  [self subclassResponsibility: _cmd];
  return nil;
}

- (NSUInteger) count
{
  [self subclassResponsibility: _cmd];
  return (NSUInteger)0;
}

- (NSUInteger) countByEnumeratingWithState: (NSFastEnumerationState*)state 	
				   objects: (id*)stackbuf
				     count: (NSUInteger)len
{
  [self subclassResponsibility: _cmd];
  return (NSUInteger)0;
}

- (NSDictionary*) dictionaryRepresentation
{
  NSEnumerator		*enumerator;
  NSMutableDictionary	*dictionary;
  id			key;

  dictionary = [NSMutableDictionary dictionaryWithCapacity: [self count]];
  enumerator = [self keyEnumerator];
  while ((key = [enumerator nextObject]) != nil)
    {
      [dictionary setObject: [self objectForKey: key] forKey: key];
    }
  return [[dictionary copy] autorelease];
}

- (void) encodeWithCoder: (NSCoder*)aCoder
{
  [self subclassResponsibility: _cmd];
}

- (NSUInteger) hash
{
  return [self count];
}

- (id) initWithCoder: (NSCoder*)aCoder
{
  [self subclassResponsibility: _cmd];
  return nil;
}

- (BOOL) isEqual: (id)other
{
  if ([other isKindOfClass: abstractClass] == NO) return NO;
  return NSCompareMapTables(self, other);
}

- (NSEnumerator*) keyEnumerator
{
  return [self subclassResponsibility: _cmd];
}

- (NSPointerFunctions*) keyPointerFunctions
{
  return [self subclassResponsibility: _cmd];
}

- (NSEnumerator*) objectEnumerator
{
  return [self subclassResponsibility: _cmd];
}

- (id) objectForKey: (id)aKey
{
  return [self subclassResponsibility: _cmd];
}

- (void) removeAllObjects
{
  NSUInteger	count = [self count];

  if (count > 0)
    {
      NSEnumerator	*enumerator;
      NSMutableArray	*array;
      id		key;

      array = [[NSMutableArray alloc] initWithCapacity: count];
      enumerator = [self objectEnumerator];
      while ((key = [enumerator nextObject]) != nil)
	{
	  [array addObject: key];
	}
      enumerator = [array objectEnumerator];
      while ((key = [enumerator nextObject]) != nil)
	{
	  [self removeObjectForKey: key];
	}
      [array release];
    }
}

- (void) removeObjectForKey: (id)aKey
{
  [self subclassResponsibility: _cmd];
}

- (void) setObject: (id)anObject forKey: (id)aKey
{
  [self subclassResponsibility: _cmd];
}

- (NSPointerFunctions*) valuePointerFunctions
{
  return [self subclassResponsibility: _cmd];
}
@end
```



这里解释一下`[self subclassResponsibility: _cmd]`，这里的意思是让子类去实现该方法，我们使用的就是`NSMapTableMap`，哪有什么子类呢？在运行时阶段，我们去看，其实是`NSMapTable的子类` -> `NSConcreteMapTable`，



关于NSPointerFunctions：

```objective-c
typedef NS_OPTIONS(NSUInteger, NSPointerFunctionsOptions) {
    // Memory options are mutually exclusive
    
    // default is strong

    // use strong write-barrier to backing store; use GC memory on copyIn
    NSPointerFunctionsStrongMemory API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (0UL << 0),       
  
    // deprecated; uses GC weak read and write barriers, and dangling pointer behavior otherwise
    NSPointerFunctionsZeroingWeakMemory API_DEPRECATED("GC no longer supported", macos(10.5, 10.8)) API_UNAVAILABLE(ios, watchos, tvos) = (1UL << 0),  
  
    NSPointerFunctionsOpaqueMemory API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (2UL << 0),
    // free() will be called on removal, calloc on copyIn
    NSPointerFunctionsMallocMemory API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (3UL << 0),       
    NSPointerFunctionsMachVirtualMemory API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (4UL << 0),
    // uses weak read and write barriers appropriate for ARC
    NSPointerFunctionsWeakMemory API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = (5UL << 0),         
    
    // Personalities are mutually exclusive
    // default is object.  As a special case, 'strong' memory used for Objects will do retain/release under non-GC
    
    // use -hash and -isEqual, object description
    NSPointerFunctionsObjectPersonality API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (0UL << 8),
    // use shifted pointer hash and direct equality
    NSPointerFunctionsOpaquePersonality API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (1UL << 8), 
    // use shifted pointer hash and direct equality, object description
    NSPointerFunctionsObjectPointerPersonality API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (2UL << 8), 
    // use a string hash and strcmp, description assumes UTF-8 contents; recommended for UTF-8 (or ASCII, which is a subset) only cstrings
    NSPointerFunctionsCStringPersonality API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (3UL << 8),
    // use a memory hash and memcmp (using size function you must set)
    NSPointerFunctionsStructPersonality API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (4UL << 8),
    // use unshifted value as hash & equality
    NSPointerFunctionsIntegerPersonality API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (5UL << 8),        
    // the memory acquire function will be asked to allocate and copy items on input
    NSPointerFunctionsCopyIn API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = (1UL << 16),      
};
```



#### NSConcreteTable.m的研究一：初始化

-------

```objective-c
- (id) initWithKeyPointerFunctions: (NSPointerFunctions*)keyFunctions
         valuePointerFunctions: (NSPointerFunctions*)valueFunctions
              capacity: (NSUInteger)initialCapacity
{
  static NSConcretePointerFunctions    *defaultFunctions = nil;
  
  // 默认是强引用
  if (defaultFunctions == nil)
    {
      defaultFunctions
    = [[NSConcretePointerFunctions alloc] initWithOptions: 0];
    }
  legacy = NO;

  if (![keyFunctions isKindOfClass: [NSConcretePointerFunctions class]])
    {
      keyFunctions = defaultFunctions;
    }
    
  memcpy(&self->cb.pf.k, &((NSConcretePointerFunctions*)keyFunctions)->_x,
    sizeof(self->cb.pf.k));

  if (![valueFunctions isKindOfClass: [NSConcretePointerFunctions class]])
    {
      valueFunctions = defaultFunctions;
    }
  memcpy(&self->cb.pf.v, &((NSConcretePointerFunctions*)valueFunctions)->_x,
    sizeof(self->cb.pf.v));

#if    GC_WITH_GC
  if (self->cb.pf.k.usesWeakReadAndWriteBarriers)
    {
      if (self->cb.pf.v.usesWeakReadAndWriteBarriers)
    {
      zone = (NSZone*)nodeWW;
    }
      else
    {
      zone = (NSZone*)nodeWS;
    }
    }
  else
    {
      if (self->cb.pf.v.usesWeakReadAndWriteBarriers)
    {
      zone = (NSZone*)nodeSW;
    }
      else
    {
      zone = (NSZone*)nodeSS;
    }
    }
#endif
  // 最后创建了一个`GSIMapTable`
  GSIMapInitWithZoneAndCapacity(self, zone, initialCapacity);
  return self;
}
```

`GSIMapTable`如下：

```objective-c
struct	_GSIMapTable {
  NSZone	*zone;
  uintptr_t	nodeCount;	/* Number of used nodes in map. map中的节点数量	*/
  uintptr_t	bucketCount;	/* Number of buckets in map.	map中的桶数量*/
  GSIMapBucket	buckets;	/* Array of buckets.		tong*/
  GSIMapNode	freeNodes;	/* List of unused nodes.	*/
  uintptr_t	chunkCount;	/* Number of chunks in array.	*/
  GSIMapNode	*nodeChunks;	/* Chunks of allocated memory.	*/
  uintptr_t	increment;
#ifdef	GSI_MAP_EXTRA
  GSI_MAP_EXTRA	extra;
#endif
};

struct	_GSIMapBucket {
  uintptr_t	nodeCount;	/* Number of nodes in bucket.	*/
  GSIMapNode	firstNode;	/* The linked list of nodes.	*/
};

struct	_GSIMapNode {
  GSIMapNode	nextInBucket;	/* Linked list of bucket.	*/
  GSIMapKey	key;
#if	GSI_MAP_HAS_VALUE
  GSIMapVal	value;
#endif
};

// GSIMapValue
typedef	union {
  NSUInteger    addr;	/* Always present */
#if	((GSUNION_TYPES) & GSUNION_OBJ)
  id		obj;
  NSObject	*nso;
#endif
#if	((GSUNION_TYPES) & GSUNION_CLS)
  Class		cls;
#endif
#if	((GSUNION_TYPES) & GSUNION_SEL)
  SEL		sel;
#endif
#if	((GSUNION_TYPES) & GSUNION_NSINT)
  NSInteger 	nsi;
  NSUInteger	nsu;
#endif
#if	((GSUNION_TYPES) & GSUNION_PTR)
  void		*ptr;
  const void	*cptr;
  char		*str;
  const char	*cstr;
#endif

/* Warning ... if this value is declared in the union, and its type is not
 * the same size as a pointer, then care must be taken in case of confusion
 * caused when an assignment to a variable using one of the union's types
 * causes part of the variable to ebe left with undefined content from the
 * point of view of another of the union's types.
 */
#if	defined(GSUNION_EXTRA)
  GSUNION_EXTRA	ext;
#endif
} GSUNION;
```





#### NSConcreteTable.m的研究二：`- (void) setObject: (id)anObject forKey: (id)aKey`

-----------

```objective-c
- (void) setObject: (id)anObject forKey: (id)aKey
{
  GSIMapNode    node;

  if (aKey == nil)
    {
      [NSException raise: NSInvalidArgumentException
          format: @"[%@-%@:] given nil argument",
        NSStringFromClass([self class]), NSStringFromSelector(_cmd)];
    }
  node = GSIMapNodeForKey(self, (GSIMapKey)aKey);
  if (node)
    {
      if (GSI_MAP_READ_VALUE(self, &node->value).obj != anObject)
    {
          GSI_MAP_RELEASE_VAL(self, node->value);
          GSI_MAP_WRITE_VAL(self, &node->value, (GSIMapVal)anObject);
          GSI_MAP_RETAIN_VAL(self, node->value);
      version++;
    }
    }
  else
    {
      GSIMapAddPair(self, (GSIMapKey)aKey, (GSIMapVal)anObject);
      version++;
    }
}

- (NSPointerFunctions*) valuePointerFunctions
{
  NSConcretePointerFunctions    *p = [NSConcretePointerFunctions new];

  p->_x = self->cb.pf.v;
  return [p autorelease];
}
```

