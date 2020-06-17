## NSUrlSession文件下载

文件下载主要分为以下场景:

- 前台下载：下载开始->下载结束

  ```mermaid
  graph LR
  A[Stard Download] -->B[Finish Download]
  ```

- 前台下载：下载开始->下载取消->重新恢复下载->下载结束

  ```mermaid
  graph LR
  A[Start Download] --> B[Cancel Download] --> C[Resume Download] --> D[Finish Download]
  ```

- 前台下载：下载开始->下载失败->重新恢复下载->下载结束

  ```mermaid
  graph LR
  A[Start download] --> B[Download Failed] --> C[Resume Dowload] --> D[Download Finish]
  ```

- 前台下载：下载开始->应用退出->应用程序打开->重新恢复下载->下载结束

  ```mermaid
  graph LR
  A[Stard Download] --> B[App Exit] --> C[App Open] --> D[Download Resume] --> E[Download Finish]
  ```

- 前台下载开始->下载失败->应用退出->应用重新打开->重新恢复下载->下载结束

