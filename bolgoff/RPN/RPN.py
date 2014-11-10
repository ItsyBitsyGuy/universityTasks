import re

postfix = []
stack = []

#some defines
operator = 10
operand = 20
leftBracket = 30
rigthBracket = 40
empty = 50

def priority(s):
    if s is '(':
        return 0
    elif s is '+' or s is '-':
        return 1
    elif s is '*' or s is '/' or s is '%':
        return 2
    else:
        return 99

def typeOf(s):
    if s is '(':
        return leftBracket
    elif s is ')':
        return rigthBracket
    elif s is '+' or s is '-' or s is '*' or s is '%' or s is '/' or s is '^':
        return operator
    elif s is ' ':
        return empty
    else:
        return operand

#infix to postfix
infix = input("Enter expression : ")
infix = infix.replace('(', '( ').replace(')', ' )').split()

for i in infix:
    type = typeOf(i)
    if type is leftBracket:
        stack.append(i)
    elif type is rigthBracket:
        next = stack.pop()
        while next is not '(':
            postfix.append(next)
            next = stack.pop()
    elif type is operand:
        postfix.append(i)
    elif type is operator:
        p = priority(i)
        while len(stack) is not 0 and p <= priority(stack[-1]):
            postfix.append(stack.pop())
        stack.append(i)
    elif type is empty:
        continue

while len(stack) > 0:
    postfix.append(stack.pop())

print(postfix)
postfix = list(reversed(postfix))
stack = []

#let's calculate
while postfix:
    a = postfix.pop()
    if a.isdigit() or a[0] == '-' and len(a) > 1:
        #print('* ' + a)
        stack.append(a)
    else:
        #print('** ' + a)
        o1, o2 = stack.pop(), stack.pop()
        if a == '^':
            stack.append(eval('(' + str(o2) + ')' + '**' + '(' + str(o1) + ')'))
        else:
            stack.append(eval(str(o2) + a + str(o1)))

print(stack[0])