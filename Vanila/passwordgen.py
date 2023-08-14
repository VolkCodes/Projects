# Welcome to a Simple Password Generator Application!
# In this python file, a user may generate a random password by first defining its length, and then the program will conjure up a random password based upon list-->string combinations
# Enjoy!

import random
import string

p_length = int(input("How long do you want your password to be, input an integer, no more than 65 please: "))

# random word generator as list

list_of_alphabets = list(string.ascii_letters)


# random letter generator as list

list_of_random_numbers = list(range(0, 10))


str_alph = []
for num in list_of_random_numbers:
    str_alph.append(str(num))


#special characters

special_characters = ["!", "@", "#", "$", "%", "^", "&", "*", "_", "-", "+", "=", "<", ">", "?"]


# append list shuffle the newly created list

str_alph.extend(list_of_alphabets)
str_alph.extend(special_characters)

combined_list = str_alph

random.shuffle(combined_list)


while len(combined_list) > p_length:
    combined_list.pop()

random_password = "".join(combined_list)

print(random_password)
