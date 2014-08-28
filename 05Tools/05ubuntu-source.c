sudo vi /etc/apt/sources.list
sudo apt-get update

#台湾源  
deb http://tw.archive.ubuntu.com/ubuntu/ precise main universe restricted multiverse  
deb-src http://tw.archive.ubuntu.com/ubuntu/ precise main universe restricted multiverse  
deb http://tw.archive.ubuntu.com/ubuntu/ precise-security universe main multiverse restricted  
deb-src http://tw.archive.ubuntu.com/ubuntu/ precise-security universe main multiverse restricted  
deb http://tw.archive.ubuntu.com/ubuntu/ precise-updates universe main multiverse restricted  
deb-src http://tw.archive.ubuntu.com/ubuntu/ precise-updates universe main multiverse restricted 

#网易 Ubuntu 11.10 源（速度很快）  
deb http://mirrors.163.com/ubuntu/ precise main universe restricted multiverse  
deb-src http://mirrors.163.com/ubuntu/ precise main universe restricted multiverse  
deb http://mirrors.163.com/ubuntu/ precise-security universe main multiverse restricted  
deb-src http://mirrors.163.com/ubuntu/ precise-security universe main multiverse restricted  
deb http://mirrors.163.com/ubuntu/ precise-updates universe main multiverse restricted  
deb http://mirrors.163.com/ubuntu/ precise-proposed universe main multiverse restricted  
deb-src http://mirrors.163.com/ubuntu/ precise-proposed universe main multiverse restricted  
deb http://mirrors.163.com/ubuntu/ precise-backports universe main multiverse restricted  
deb-src http://mirrors.163.com/ubuntu/ precise-backports universe main multiverse restricted  
deb-src http://mirrors.163.com/ubuntu/ precise-updates universe main multiverse restricted 

#骨头源，骨头源是bones7456架设的一个Ubuntu源 ，提供ubuntu,deepin 
deb http://ubuntu.srt.cn/ubuntu/ precise main universe restricted multiverse  
deb-src http://ubuntu.srt.cn/ubuntu/ precise main universe restricted multiverse  
deb http://ubuntu.srt.cn/ubuntu/ precise-security universe main multiverse restricted 
deb-src http://ubuntu.srt.cn/ubuntu/ precise-security universe main multiverse restricted  
deb http://ubuntu.srt.cn/ubuntu/ precise-updates universe main multiverse restricted  
deb http://ubuntu.srt.cn/ubuntu/ precise-proposed universe main multiverse restricted  
deb-src http://ubuntu.srt.cn/ubuntu/ precise-proposed universe main multiverse restricted 
deb http://ubuntu.srt.cn/ubuntu/ precise-backports universe main multiverse restricted  
deb-src http://ubuntu.srt.cn/ubuntu/ precise-backports universe main multiverse restricted  
deb-src http://ubuntu.srt.cn/ubuntu/ precise-updates universe main multiverse restricted 

#mirror.lupaworld.com的源，速度很快 
deb http://mirror.lupaworld.com/ubuntu/archive/ precise main restricted universe multiverse 
deb http://mirror.lupaworld.com/ubuntu/archive/ precise-security main restricted universe multiverse 
deb http://mirror.lupaworld.com/ubuntu/archive/ precise-updates main restricted universe multiverse 
deb http://mirror.lupaworld.com/ubuntu/archive/ precise-backports main restricted universe multiverse 
deb http://mirror.lupaworld.com/ubuntu/ubuntu-cn/ precise main restricted universe multiverse 