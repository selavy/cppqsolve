trade = True;

def initialize(context):
    context.gs = 'GS'

def strategy(context):
    global trade
    if trade:
        context.order('AAPL', 50);
    else:
        context.order('AAPL', -50);
    trade ^= True;
