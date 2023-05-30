import flexmc as mc

test = mc.Token(mc.Token.Type_eof)
y = test
x = "test myKeyword x = 5.00"

lexer = mc.Lexer(x)

first_token = lexer.nextToken()

while first_token.type != mc.Token.Type_eof:
    print(first_token.toString())
    first_token = lexer.nextToken()
print('final')
print(lexer.nextToken().toString())
lexer.reset()
