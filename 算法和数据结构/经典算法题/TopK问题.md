## TopK问题

--------

N个数中，找前K个数。

有两种解题思路：

- 快排
- 大顶堆或者小顶堆

对于大数据量的来说，需要使用大顶堆或者小顶堆来解决问题



#### 快排

-------

主要分为以下几个步骤：

- 随机找一个值，比如第一个值Array[0]
- 从前向后遍历，如果小于Array[0]交换，遍历完成后，左侧的数据有m个，而右侧的数据有n-m个
- 若 k == m，那么已经到了所有的k个数，就是左边的数组
- 若k < m，那么需要在左侧再次使用快排
- 若k > m，那么需要在右侧找前`k -m`个元素

```swift

func getLeastNumbers(_ numbers:[Int], _ k:Int) -> [Int] {
    
    if k == 0 || numbers.count == 0 {
        return [Int]()
    }
    
    if k >= numbers.count {
        return numbers
    }
    
    var minNumbers = [Int]()
    var maxNumbers = [Int]()
    let mid = numbers.count / 2
    
    for i in 0..<numbers.count {
        if numbers[i] < numbers[mid] {
            minNumbers.append(numbers[i])
        }
        else {
            maxNumbers.append(numbers[i])
        }
    }
    
    if minNumbers.count == k {
        return minNumbers;
    }
    else if minNumbers.count > k {
        return getLeastNumbers(minNumbers, k)
    }
    else if minNumbers.count < k {
        let tmpArr = getLeastNumbers(maxNumbers, k - minNumbers.count)
        minNumbers.append(contentsOf: tmpArr)
        return minNumbers
    }
    
    return minNumbers
}
```





#### 小顶堆

-------

