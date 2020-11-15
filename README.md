# UPDATED README AND INSTRUCTIONS
# ORIGINAL : https://www.tuxad.com/blog/archives/2019/09/20/workaround_for_unity_installation_issue_not_enough_space/index.html
<p>Whoever is the hero who made this, is a true champion!</p>
So, in order to run this:
<li> Move to the directory you downloaded the app image 
<li> once you see your UnityHub.AppImage do

```shell script
./launch-uhs-with-faked-increased-diskspace.sh ./UnityHub.AppImage
```

Once you do that the first time and UnityHub runs, break in the terminal by pressing ctrl + c.
<br>
Once that is done as well, run the same command again and install Unity Editor yout wanted.
```shell script
./launch-uhs-with-faked-increased-diskspace.sh ./UnityHub.AppImage
```
------------------------------------------------------------------------------
UnityHubSetup.AppImage has a bug which causes issues on many installations:
It always checks the root file system ("/") for free space - even if you don't want to install in root fs.
And many root fs have only a small amount of free disk space because many folders use their own partitions and fs.
And if you have only 2 gigs free on root fs then installer is causing issues if you try to install more than 2 gigs.

As a workaround I created a wrapper library which fakes all results of statfs() calls making the installer think that you have sufficient disk space.
Unfortunately UnityHubSetup.AppImage is not the actual installer. It extracts a temporary image directory in /tmp and then starte the installer in it.
That means that we'll have to start UnityHubSetup.AppImage to be able to copy this image directory, do our fake patched in it and then start the
installer from this directory.
This is done by launch-uhs-with-faked-increased-diskspace.sh.

Usage:
  launch-uhs-with-faked-increased-diskspace.sh [path/to/UnityHubSetup-Image]
You can copy the install image to this dir or specify the path to it as command line argument.
Example1:
  ./launch-uhs-with-faked-increased-diskspace.sh ../Unity\ Hub/UnityHubSetup.AppImage
Example2:
  cp -af ../Unity\ Hub/UnityHubSetup.AppImage .
  ./launch-uhs-with-faked-increased-diskspace.sh
  
The Installer will be started TWO times! The first run is just for unpacking. Please wait for second run for installation.

FWB 9/2019
www.tuxad.com
