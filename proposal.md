---
permalink: /proposal/
---

## Proposal

> Yifan Ren, Yang Heng

### Problem Description

Cubic style have been attracting artists’ attention over centuries. However, apart from those image stylization filters and non-realistic rendering methods, direct stylization and modeling algorithm has received less attention. Our group aims to generate 3D cubic stylized shape and create cubic stylized scene. It is always hard to preserve geometry details while stylization because cubifying a shape leads to loss of details. We are going to use HTD Liu's state of the art algorithm as our primary resource.

### Goals and Deliverbles

For this project, we are going to deliver a series of stylized shapes and some scenes consisting of those shapes. We will first generate some cubic stylized shapes. Later we will take some typical scenes as our source and generate colored cubic stylized scenes by our approach.

During this project, we need to figure out how to preserve geometry details while cubifying a shape. So we will compare the Liu's algorithm with those traditional algorithm and find the main factor. Then we are going to test some energy functions during stylization to optimize our approach.

At the mean time, we want to add some features during the stylization process. For example, we want to test stylization for specific axis and add some new constraints.

We also hope to capture different styles, maybe create different styles not limited to cubism style.

However, there is no standard to evaluate how beautiful our output is. But we will evaluate our approach's robustness by some corner data case. And evaluate the performance of our output subjectively.

### Schedule

Week 1: Read through papers related to cubic stylization and get acquainted with the use of modeling and rendering tools.

Week 2~3: Implement Liu's algorithm and traditional algorithm and make a comparison.

Week 4~5: Add some new features and constraints to the original process. And try to capture different styles if we have more time.

Week 6: Generate stylized shapes and scenes and do some analysis.

### Resources

> https://www.dgp.toronto.edu/projects/cubic-stylization/
> https://web.stanford.edu/~boyd/papers/pdf/admm_distr_stats.pdf
> https://www.dgp.toronto.edu/projects/paparazzi/
> http://www.geometry.caltech.edu/pubs/HJSTBD13.pdf

