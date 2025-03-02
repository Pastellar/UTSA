#   Divide and Conquer
    For this program, the input is an array of doubles, where each
    entry A[i] denotes the price of a stock on the ith day.
    Example: if A = [5, 2, 3, 6, 1, 3] then this means that the 
    stock in question was worth 5 on day 0, it was worth 3 on day 2, etc.

    If the stock is bought on buyDay and sold on sellDay, then the goal
    is to maximize the profit. A[sellDay] – A[buyDay]
    
    Using a divide and conquer algorithm, the program attempts to achieve
    optimal profit based on provided data. Ideally: buyDay < sellDay.
    Stock is not sold on the same day it is bought.

    Written for our Analysis of Algorithms class.

##  Authors
    - Haven Kotara      https://github.com/Pastellar
    - Andrew McNeil     https://github.com/BelowAverageCoder