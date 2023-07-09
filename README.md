# bigint
It was my ambitious project to implement arbitrary-size integer.  
My plan was to use a linked list of arrays for its internal data structure, because I thought using one dynamic array will cost a lot when re-allocating array,
and using a linked list of say, `int` will cost a lot due to frequent calls of `malloc()`. So I chose something in the middle.  
Anyway, I gave up because it felt somewhat dirty and meaningless so this project is not completed yet.
