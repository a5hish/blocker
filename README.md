# blocker
A C-based tool which enable's you to block Website's and Application's on your linux based operating system.
### Installation (using make)

| make Options | Description |
| ------------ | ----------- |
| make | compile and build the tool.|
| make clean |  removes the object file's created during make |
| make install | install's the tool [i.e: it create's a directory for it and put's the object file for the 'blocker' in it and also create the default configuration file (.blockrc) for the tool]|
| make uninstall |removes the directory created during 'make install' [if in the same folder.]|
#### How to compile and install
```sh
$ cd blocker
$ make 
$ make install
```

## Description
***blocker*** block's website's and application's which are black-listed/specified in the configuration file(.blockrc)

| Options | usage |
| ---- | ---- |
| -help | show help options available with the tool. |
| -block |  block's all the urls/application specified in the configuration file. |
| -unblock | Unblock's the urls/application specified in the configuration file |
| -edit | open's the configuration file to edit in an editor |

>Note : the tools should be used with root privilege as it manipulate's the /etc/hosts file.

#### About configuration file
The url's of websites and path's to the application binary should be specified in the configuration. file .

Example:
open the config file by using the -edit option.
```sh
$ ./blocker -edit
```
To block fb.com and netflix.com edit the config file 
after editing it should look like this.
```sh
[URLS]
# enter your urls below this comments. 
# (eg: www.example.com)
www.fb.com
www.netflix.com

[PATHS]
# enter path to the BINARY of the application to block below this comments.
# (eg: /home/application/app)
```
Save the config file and exit.
and then use the following option to block them.
```sh
$ cd undefined
s sudo ./blocker -block
```
***(done they are blocked now)***
to unblock use the -unblock option.
```sh
$ sudo ./blocker -unblock
```
