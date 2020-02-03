# `Orderer`
The `Orderer` is a pin-pointed generic functionality built to order any object based on a user-specified criterion. 

## Problem Statement
We needed a packet ordering functionality on a high-speed packet route. Packets could come in unordered but the system had to send them out ordered. 

## Solution
Since the packets could come in out of order, keeping them ordered would be too much work. So, it is not an option to keep the packets in order. Moving existing entries when inserting in the middle of a `std::vector` is not helpful. Nor does `std::map` cut it due to cache unfriendly behaviour. 

#### LAZY is the key! 
The solution is to have an unsorted `std::vector`! :stuck_out_tongue: 
The `Orderer` just puts new packets at the end and does not order them until needed. 
The user can ask for a consuming function to be executed on all the 'ready' packets. The packets will be ordered only when the user asks it to using the `invoke_on_ready` function. 

## Compilation
This is header a only implementation. 
The implementation is C++17 compatible. 

Test execution can be done with the following command: 
```bash
$ CC -std=c++17 -I../ random_rx_tx.cc -o random_rx_tx
$ ./random_rx_tx
```

## API
The public interface: 

```C++ 
template<typename T, typename Criterion_type/*uint64_t or chrono::microseconds latency_duration*/>
class Orderer
```
Template Parameters: 

`T` - the data-type of items to be ordered

`Criterion_type` - data-type of the ordering & qualifying criterion 

`insert`
Add a new `T` to the `Orderer` internal container

`invoke_on_ready`

`threshold` - threshold value of the criterion to assess which `T`s are ready

`call` - a Callable that will be invoked for all ready `T`s

Return - count of items processed
