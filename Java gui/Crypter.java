/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Abhi
 */
public class Crypter {

/**

* Encrypt using Vigenere cipher

* @param s open text

* @param key key phrase (only capital letters)

* @return ciphertext (only capital letters)

*/

public static String encipher(String s, String key){

    StringBuilder builder = new StringBuilder();
    
    for(int i = 0; i < s.length(); i ++)
    {
        
        if(s.charAt(i) < 65 || s.charAt(i) > 90)
        { //ASCII character (capital letter)
            throw new IllegalArgumentException("" +"Open text must contain only capital letters");

        }

    //add shift modularly
        
    char encyphered = s.charAt(i) + getShift(key, i) > 90 ? (char)((s.charAt(i) + getShift(key, i)) - 26) : (char)(s.charAt(i) + getShift(key, i));
    
    builder.append(encyphered);
    
}

return builder.toString();

}

/**

* Decrypt using Vigenere cipher

* @param s cipher text (only capital letters)

* @param key key phrase (only capital letters)

* @return open text

*/

public static String decipher(String s, String key){

StringBuilder builder = new StringBuilder();

for(int i = 0; i < s.length(); i ++){

if(s.charAt(i) < 65 || s.charAt(i) > 90){ //ASCII character (capital letter)

throw new IllegalArgumentException("" +"Ciphertext must contain only capital letters");

}

//subtract shift modularly

char decyphered = s.charAt(i) - getShift(key, i) < 65 ? (char)((s.charAt(i) - getShift(key, i)) + 26) : (char)(s.charAt(i) - getShift(key, i));

builder.append(decyphered);

}

return builder.toString();

}

/**
45.
* Get shift
46.
* @param key key phrase
47.
* @param i position in the text
48.
* @return shift
49.
*/

private static int getShift(String key, int i) 
{

    if(key.charAt(i % key.length()) < 65 || key.charAt(i % key.length()) > 90)
    {
    
        throw new IllegalArgumentException("" +"Key phrase must contain only capital letters");
    
    }
    
    return ((int)key.charAt(i % key.length())) - 65;
    
    }

}