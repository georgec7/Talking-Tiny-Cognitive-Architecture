# Talking-Tiny-Cognitive-Architecture
The Talking Tiny Cognitive Architecture is a system that senses a data from sensors such as 
temperature, light and differentiates the data based on range and creates the self-vocabulary table. Also, 
it communicates with other system with same environmental conditions and creates listening 
vocabulary table while communicating with other system.
Initially both systems are stationed in the same room in the same environment. Each system learns the 
other systems behavior of generating the words that denotes a temperature or light or temperature and 
light condition. Based on the learnings attained, both the systems should be now able to predict the 
words transmitted between each other when placed in different environments using Naïve Bayes 
Classifier.
The system performs the following operations: 1) Sensing 2) Learning and 3) Decision making.
Below is a flow chart of our algorithm which gives a high-level idea of how our design performs the 
above three operations

![image](https://user-images.githubusercontent.com/49031532/212646053-d204b955-b629-4804-bd92-695e1319abda.png)
