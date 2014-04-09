from qsolve import *

trade = True;

def initialize(context):
    context.gs = 'GS'

def strategy(date):
    global trade
    if trade:
        order('AAPL', 50);
    else:
        order('AAPL', -50);
    trade ^= True;
