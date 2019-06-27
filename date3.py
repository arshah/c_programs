# check whether the input date is valid or not

from dateutil.parser import parse
def validate(date_text):
    try:
        parse(userdate)
    except ValueError:
        print("Invalid date")

"""
import datetime
def validate(date_text):
    try:
        datetime.datetime.strptime(date_text, '%d-%m-%Y')
    except ValueError:
        raise ValueError("Incorrect date format")
"""



# function to produce the next date

#def nextdate (ch):
    #date=input("Enter the date: ")
    #dd,mm,yy=userdate.split('ch')




# Get user input date and pass it to other functions

import re
userdate = input("Enter the date ")
validate(userdate)
c1 = '-'
c2 = '/'
isSepeartionDash = False

if c1 in userdate:
    dd,mm,yy=userdate.split('-')
    dd=int(dd)
    mm=int(mm)
    yy=int(yy)
    isSepeartionDash = True
elif c2 in userdate:
    dd,mm,yy=userdate.split('/')
    dd=int(dd)
    mm=int(mm)
    yy=int(yy)
    isSepeartionDash = False
#else print("Invalid input")

printSepeartionChar = c1 if isSepeartionDash else c2

if(mm==1 or mm==3 or mm==5 or mm==7 or mm==8 or mm==10 or mm==12):
    max1=31
elif(mm==4 or mm==6 or mm==9 or mm==11):
    max1=30
elif(yy%4==0 and yy%100!=0 or yy%400==0):
    max1=29
else:
    max1=28
if(mm<1 or mm>12):
    print("Date is invalid.")
elif(dd<1 or dd>max1):
    print("Date is invalid.")
elif(dd==max1 and mm!=12):
    dd=1
    mm=mm+1
    print("The incremented date is: %d%s%d%s%d" % (dd,printSepeartionChar,mm,printSepeartionChar,yy))
elif(dd==31 and mm==12):
    dd=1
    mm=1
    yy=yy+1
    print("The incremented date is: %d%s%d%s%d" % (dd,printSepeartionChar,mm,printSepeartionChar,yy))
else:
    dd=dd+1
    print("The incremented date is: %d%s%d%s%d" % (dd,printSepeartionChar,mm,printSepeartionChar,yy))

