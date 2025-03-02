import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashSet;

public class Main {
	static HashSet<String> dict = new HashSet<String>();
	
	public static void main(String[] args) {
		try {
			//used to read file line by line
			//open dictionary file
			File fileIn = new File("files/aliceInWonderlandDictionary.txt");
			FileReader fr = new FileReader(fileIn);
			BufferedReader br = new BufferedReader(fr);
			
			//store every word in the dict HashSet
			String word = "";
			while((word = br.readLine()) != null) {
				dict.add(word);
			}
			
			//open the input file
			fileIn = new File("files/input.txt");
			
			br.close();
			fr.close();
			
			fr = new FileReader(fileIn);
			br = new BufferedReader(fr);
			
			//for each string/line in the input file
			while((word = br.readLine()) != null) {
				//reinitialize and resize 2D int a String arrays used to store fewest words in each substring
				int[][] c = new int[word.length()][word.length()];
				String[][] remade = new String[word.length()][word.length()];
				
				//fill every element in c array with -1 by default
				for(int[] row : c) {
					Arrays.fill(row, -1);
				}
				
				//find the reconstructed string using the fewest valid words if possible
				String result = fewestWords(word, c, remade);
				
				int AiWs;
				//if it the returned string is valid and could be split
				if(result != null) {
					//used to count how many AiWs in the reconstructed string
					AiWs = result.split(" ").length;
				}
				else
					AiWs = 0;
				
				if(AiWs > 0)
					System.out.println(word + " can be split into " + AiWs + " AiWs words: " + result);
				else
					System.out.println(word + " cannot be split into AiWs words.");
				
				c = null;
				remade = null;
			}
			
			//close the input readers
			br.close();
			fr.close();
		}
		catch(IOException e) {
			e.printStackTrace();
		}
	}
	
	//bottom-up dynamic programming method to find the least amount of words in a given string
	//String phrase is the given string, int[][] c stores the smallest number of words in a given substring,
	//String[][] remade stores the reconstructed string using the fewest words within a given substring
	//returns phrase broken up into the fewest words (remade[0][phrase.length()-1])
	public static String fewestWords(String phrase, int[][] c, String[][] remade) {
		for(int i = 0; i < phrase.length(); i++) {
			//first index of the substring
			for(int j = 0; j < phrase.length()-i; j++) {
				//last index of substring + 1 (so substring is substring(inclusive, inclusive))
				int h = i + j + 1;
				//base case 1
				//if substring from j to h is a single word
				if(dict.contains(phrase.substring(j, h))) {
					c[j][h-1] = 1;
					remade[j][h-1] = phrase.substring(j, h);
				}
				//base case 2
				//if the substring is a single character, but not a valid word
				else if(j == h-1) {
					c[j][h-1] = 0;
				}
				//else dynamically find least num of words by breaking the substring down into previously calculated substrings
				else {
					//used for inner substring boundary
					for(int k = j; k < h-1; k++) {
						//if both inner substrings are made of valid words
						if(c[j][k] > 0 && c[k+1][h-1] > 0) {
							//if the substring has not yet been calculated or if the inner substrings contain fewer words than a previous calculation
							if(c[j][h-1] < 1 || (c[j][k] + c[k+1][h-1]) < c[j][h-1]) {
								c[j][h-1] = c[j][k] + c[k+1][h-1];
								remade[j][h-1] = remade[j][k] + " " + remade[k+1][h-1];
							}
						}
					}
				}
			}
		}
		//if phrase can be split into valid words
		if(c[0][phrase.length()-1] > 0)
			return remade[0][phrase.length()-1];
		else
			return null;
	}
}
