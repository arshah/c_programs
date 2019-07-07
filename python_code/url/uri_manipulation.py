import uriModule

url = input("Enter a URL: ")
print(" \n Enter any of the following numbers to get parts of the URI/URL : ")
n = int(input(" 1 for scheme \n 2 for domain \n 3 for directory/path \n 4 for sub-directory/query \n 5 page/fragment\n\n "))

print(uriModule.uriManipulation(url, n))

