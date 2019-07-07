import unittest
from uriModule import uriManipulation
import re

class TestUriManipulation(unittest.TestCase):

    #def test(self):
        #self.assertTrue(True)

    def test_values(self):
        self.assertRaises(ValueError, uriManipulation, -2)  # checking valueError for negative integer input
        self.assertRaises(ValueError, uriManipulation, 0)   # checking valueError for 0


    def test_types(self):
        self.assertRaises(TypeError, uriManipulation, 1.1)    # checking TypeError for float input
        self.assertRaises(TypeError, uriManipulation, "h")    # checking TypeError for character input
        self.assertRaises(TypeError, uriManipulation, "ii")   # checking TypeError for string input
        self.assertRaises(TypeError, uriManipulation, "-")    # chceking TypeError for special character input


if __name__ == "__main__":
    unittest.main()
