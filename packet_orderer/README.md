# `Orderer`
The `Orderer` is a pin-pointed generic functionality built to order any object based on a user-specified criterion. 

## Problem Statement
We needed a packet ordering functionality on a high-speed packet route. Packets could come in unordered but the system had to send them out ordered. 

## Solution
Since the packets could come up out of order, keeping them ordered would be too much work. So, it is not an option to keep the packets in order. Moving existing entries when inserting in the middle of a `std::vector` is not helpful. Nor does `std::map` cut it due to cache unfriendly behaviour. 

### LAZY is the key! 
The solution is to have an unsorted `std::vector`! :P 
The `Orderer` just puts new packets at the end and does not order them until needed. 
The user can ask for a consuming function to be executed on all the 'ready' packets. 
