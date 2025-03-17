# generic-programming
A short introduction in generic programming in modern C++ with a nice list of references.

## Requirements
* g++ >= 14
* It should also compile with the latest versions of clang or MSVC, when using the c++23 standard. If it doesn't, use the `no_modules` version.

<details>
<summary>For Windows: WSL</summary>
<br>
<ol>
  <li><a href="https://learn.microsoft.com/en-us/windows/wsl/install">https://learn.microsoft.com/en-us/windows/wsl/install</a></li>
  <li><a href="https://code.visualstudio.com/docs/remote/wsl">https://code.visualstudio.com/docs/remote/wsl</a></li>
  <li>See Linux Setup</li>
</ol>
</details>

<details>
<summary>Linux Setup (Ubuntu)</summary>
<br>
<pre>
sudo apt-get update
sudo apt-get install build-essentials
sudo apt install g++-14
</pre>
</details>

## Contents
0. What is Generic Programming: [0.short-story](./0.short-story/).
1. Introduction to C++20 modules: [1.modules](./1.modules/).
2. Introduction to concepts: [2.concepts](./2.concepts/).
3. Iterators: [3.iterators](./3.iterators/).
4. Minimum and maximum: [4.minmax](./4.minmax/).
5. Minimum and maximum: [5.parallel](./5.parallel/).

***

## Resources
* [21st Century C++](https://cacm.acm.org/blogcacm/21st-century-c/), by Bjarne Stroustrup.
* The [Awesome Modern C++](https://awesomecpp.com/) list.
* [Generic Programming](https://sean-parent.stlab.cc/papers-and-presentations/#generic-programming).
* Alexander A. Stepanov: Three Algorithmic Journeys. Lectures presented at A9.com, 2012.
    * I. Spoils of the Egyptians: [Videos](https://www.youtube.com/playlist?list=PLHxtyCq_WDLV5N5zUCBCDC2WqF1VBDGg1) / [PDF](https://www.stepanovpapers.com/Journeys/Journey1.pdf).
    * II. Heirs of Pythagoras: [Videos](https://www.youtube.com/playlist?list=PLHxtyCq_WDLW0NqZCcrrQUa24H_af6Mrn) / [PDF](https://www.stepanovpapers.com/Journeys/Journey2.pdf).
    * III. Successors of Peano: [Videos](https://www.youtube.com/playlist?list=PLHxtyCq_WDLXrHwcaay14-4FXdzQBIMGx) / [PDF](https://www.stepanovpapers.com/Journeys/Journey3.pdf).
    * Epilogue: [Video](https://www.youtube.com/playlist?list=PLHxtyCq_WDLVQPzEm3igPiYOR68HQcMZT) / [PDF](https://www.stepanovpapers.com/Journeys/Epilogue.pdf).
* Alexander A. Stepanov: Efficient Programming with Components. Lectures at A9.com, 2013.
    * [Videos](https://www.youtube.com/playlist?list=PLHxtyCq_WDLXryyw91lahwdtpZsmo4BGD)
    * [GitHub](https://github.com/rjernst/stepanov-components-course)
* See https://sean-parent.stlab.cc/papers-and-presentations/.


## References
* Alexander A. Stepanov and Paul McJones: Elements of Programming. Semigroup Press, June 2019 and Addison-Wesley Professional, June 2009. 
* Alexander A. Stepanov and Daniel E. Rose: From Mathematics to Generic Programming. Addison-Wesley Professional, November 7, 2014.
* See https://www.stepanovpapers.com/.
