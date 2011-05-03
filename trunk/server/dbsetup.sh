#!/bin/bash  
#Run the script with 1 integer as parameter. The integer represents the number of database insertions.

ARGS=1
E_BADARGS=65

if [ $# -ne "$ARGS" ]
then 
  echo "Usage: `basename $0` insertnumber"
  exit $E_BADARGS
fi

if [ -f test.db ]
then
  rm test.db
fi
sqlite3 test.db "create table Player (UID varchar(20) primary key, Password varchar(20), Kills integer, Score integer, Deaths integer,imagePath varchar(100))";
i=0;
n=$1;
while [ ${i} -lt ${n} ];
do  
  echo -n "Enter user id `expr $i + 1`:";
  read userid;
  echo -n "Enter password `expr $i + 1`:";
  read password;
  echo -n "Enter imagepath `expr $i + 1`:";
  read image;
  sqlite3 test.db "insert into Player (UID,Password,Kills,Score,Deaths,imagePath) values ('$userid','$password',0,0,0,'$image')";
  i=`expr $i + 1`;

done

exit 0
