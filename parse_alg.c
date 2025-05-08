// PARSE ALGORITHM.
/**
Essentially, we are building a top-down tree dynamically from matching a linear queue of tokens (terminals) to the leaf nodes of a dynamically-graphed pathway through a set of Production Rules.
*/

1. Start at START symbol. Any segment matched from the start symbol will serve as the Root Node.

In order to match to a NON-TERMINAL symbol, we need to match any TERMINAL symbol(s) in the direct descendency graph.


Scan the segments for NONTERMINAL symbol START.

Token = EmptyToken;

SEGMENT tnt[];

TERMINAL; // a terminal is effectively a concrete instance of a lex token.
NONTERMINAL; // a nonterminal is effectively a production rule.


