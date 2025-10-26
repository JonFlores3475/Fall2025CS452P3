# Project 3

- Name: Jon Flores
- Email: jonathanflores@u.boisestate.edu
- Class: 452-001

## Known Bugs or Issues

Coverage is a bit lower than I would like, and there a lot of the ignore command calls on all the multithreaded things. There are also just some scenarios that I included that do not seem to get tested, but that I think would be necessary for a complete product.

## Experience

As I progress through this course, I am slowly but surely becoming more comfortable with the C programming language again. I was very excited to tackle this project after discussing it in class with Dr. Panter, so I had an architecture already in mind when sitting down to code. As such, I started by laying out a skeleton of what I had envisioned, then started to code it up by hand. Finally, I checked all the files through AI to check for validity in terms of syntax, made a few minor adjustments, and then repeated this process for compiler errors and warnings as they began to appear. Eventually I had a working code with minimal input from AI this time, which felt good. I had a little trouble ensuring that I used a lock in this assignment to meet the assignment criteria, as my approach did not necessarily require one, but I did get one in there.

## Analysis

- Were you able to generate soething close to what the example showed?
    - Yes, but on onyx, I had to greatly increase the size of the arrays to be sorted to generate something close to the example plot. The best results I saw were for 10000000 element arrays, but after some discussion with Dr. Panter, he confirmed that this was acceptable. There was also some jaggedness due to the nature of using onyx rather than running on a local machine.

- Did you see a slowdown at some point? Why or why not?
    - Everything followed a pretty smooth pattern of decreased run time, with a very slight upwards trend as the number of threads approached the number of cores available. I have adjusted the generateplot.sh script to go up to my max thread count to further demonstrate this trend on onxy as more cores are available at some times.

- Did your program run faster and faster when you added more threads? Why or Why not?
    - My program did run faster and faster for a good while when adding more threads, but eventually it stabilized, then, as the number of threads approached the number of cores, there was a slight upward trend appearing again.

- What was the optimum number of threads for your machine?
    - On Onyx, it seems like the optimal number of threads was 14.

- What was the slowest number of threads for your machine?
    - On Onyx, the slowest number of threads seemed to be 1.

![Onyx Plot: 64 thread max, 10000000 element arrays](scripts/64_10000000.png)
