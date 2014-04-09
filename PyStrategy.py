from qsolve import *

def initialize(context):
    context.gs = 'GS'

def strategy(date):
    order('AAPL', 510);
