#! /bin/bash


if [ "$1" == "-p" ]
    then
    for i in `ls /proc | grep -E "^[[:digit:]].*$"`
    do 
    pid=`cat /proc/$i/status 2> /dev/null | grep -E "^Pid" | cut -f 2`
    ppid=`cat /proc/$i/status 2> /dev/null | grep -E "PPid" | cut -f 2`
    uid=`cat /proc/$i/status 2> /dev/null | grep -E "Uid" | cut -f 2`
    nazwau=`cat /etc/passwd | grep -E "x:$uid:" | cut -d ":" -f 1`
    uruchomionz=`cat /proc/$i/comm 2> /dev/null | rev | cut -d "/" -f 1 | rev`
    if [[ $pid =~ [^[:space:]] ]]
    then
    echo $pid $ppid $uid"($nazwau)" $uruchomionz
    fi
    done |sort -n
elif [ "$1" == "-u" ]
then
    username=`whoami`
    userid=`cat /etc/passwd | grep "^$username" | cut -d ":" -f 3`

    for i in `ls /proc | grep -E "^[[:digit:]].*$"`
    do 
    uid=`cat /proc/$i/status 2> /dev/null | grep -E "Uid" | cut -f 2`
    if [ "$uid" == $userid ]
    then 
    pid=`cat /proc/$i/status 2> /dev/null | grep -E "^Pid" | cut -f 2`
    uruchomionz=`cat /proc/$i/comm 2> /dev/null | rev | cut -d "/" -f 1 | rev`
    katrob=`readlink -e /proc/$i/cwd`
    echo $pid $uruchomionz $katrob
    fi
    done |sort -n
else
    echo "BLAD: nie uruchomiono z przelacznikiem -p lub -u"
fi

