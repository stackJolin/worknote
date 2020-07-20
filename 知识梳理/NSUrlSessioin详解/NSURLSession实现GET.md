## NSUrlSession实现GET方法

```
// Create NSURLSession object
NSURLSession *session = [NSURLSession sharedSession];

// Create a NSURL object.
NSURL *url = [NSURL URLWithString:@"http://www.yahoo.com/download.cgi?id=1"];

// Create NSURLSessionDataTask task object by url and session object.
NSURLSessionDataTask *task = [session dataTaskWithURL:url completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        
    NSLog(@"%@",[[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding]);
    
    // Print response JSON data in the console.
    NSLog(@"%@", [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil]);

}];

// Begin task.
[task resume];
```

