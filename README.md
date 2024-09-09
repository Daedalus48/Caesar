Caesars Encryption replaces each letter in the text with the letter at a given number of steps forwards or backwards in the alfabet.

The number of steps the letters are transposed by is specified with the Increment input parameter. 
When a letter is transposed past the last letter of the alfabet it's wrapped around to the beginning and vice versa.

Capital letters stay capital and lower case letters stay lower case. Symbols that are not part of the alfabet are left unchanged.

If British is selected the last letter of the alfabet is 'Z'. If Norwegian is selected the letters 'Æ', 'Ø', 'Å' are added to the end of the alfabet and are also converted.




Brutus' Encryption uses the same method to replace letters, but instead of incrementing all letters with a fixed number, it generates a random list of numbers that can be used as an encryption key.

The first letter is incremented by the first number from the key, the second letter by the second number and so on.
