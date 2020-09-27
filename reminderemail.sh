#!/bin/bash

gcc unixproject.c -o r
./r

input="Remainder.txt"
while IFS= read -r line
do

 email="$(cut -d';' -f3 <<<"$line")"
 duedate="$(cut -d';' -f4 <<<"$line")"
 bookname="$(cut -d';' -f2 <<<"$line")"
 echo $email
 #echo $duedate
 echo "you have a book named :$bookname , due , due date :$duedate" | mail -s "book due" "$email"
 echo "mail sent"
done < $input
cat /dev/null > $input




