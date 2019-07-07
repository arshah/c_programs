# this is a module for parsing URL/URI

import re

def uriManipulation(url, num):

    if type(num) not in [int]:
        raise TypeError(" Your input must be a number\n")
    if (num < 1 or num > 5 ):
        raise ValueError(" Enter a number within the range 1 to 5\n")

    '''
    regex = r"(https?:\/\/(www\.)?)([^\/]+)\/([^\/]+)\/([^\/]+)\/(.*)"

    test_str = ("http://www.ics.some_domain.com/some_other_slash/another_slash/\n"
    "https://www.some_domain.com/some_other_slash/another_slash/\n"
    "http://some_domain.co.uk/some_other_slash/another_slash/\n"
    "https://some_domain.co.uk/some_other_slash/another_slash/more_data_here")


    subst = '\\' + str(num)

    # manually specify the number of replacements by changing the 4th argument
    result = re.sub(regex, subst, test_str, 0, re.MULTILINE)

    if result:
        print (result)

    '''

    if (num == 1):
        search_group = 2
    elif (num == 2):
        search_group = 4
    elif (num == 3):
        search_group = 5
    elif (num == 4):
        search_group = 7
    elif (num == 5):
        search_group = 9

    #  $1 = http:
    #  $2 = http (scheme)
    #  $3 = //www.ics.uci.edu
    #  $4 = www.ics.uci.edu (domain)
    #  $5 = /pub/ietf/uri/ (path)
    #  $6 = <undefined>
    #  $7 = <undefined> (query)
    #  $8 = #Related
    #  $9 = Related (fragment)
    return re.fullmatch("^(([^:/?#]+):)?(\/\/([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?", url).group(search_group))
