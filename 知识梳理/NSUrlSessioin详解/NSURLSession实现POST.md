## NSURLSession实现POST

```
// Create NSURLSession object.
NSURLSession *session = [NSURLSession sharedSession];

// Create a NSURL object.
NSURL *url = [NSURL URLWithString:@"http://www.yahoo.com/regist"];

// Create post request object with the url.
NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
// Set request method to POST.
request.HTTPMethod = @"POST";
// Set post username and password data.
request.HTTPBody = [@"username=Richard&pwd=888888" dataUsingEncoding:NSUTF8StringEncoding];

// Create the NSURLSessionDataTask post task object.
NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {

        NSLog(@"%@",[[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding]);
        
        // Print out the result JSON format data. 
        NSLog(@"%@", [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil]);

        }];

// Execute the task
[task resume];
```

