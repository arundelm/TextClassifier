# C++ Automated Market Text Classification Model
This project is a Machine Learning (ML) and Natural Language Processing (NLP) model that uses a Bag of Words approach to classify financial statements. It achieves 90% accuracy by analyzing the content of financial datasets and determining the subject of each sentence.

## Project Overview
The main goal of this project is to build a text classifier that processes sentences about financial markets and predicts which aspect of the market the sentence pertains to. The project reads datasets containing tagged financial statements, analyzes the text using a Bag of Words model, and then classifies new sentences based on their content.

## Key Features
Bag of Words Model: The classifier tokenizes the input text and removes duplicates to analyze word frequency and associations.

Log Prior and Log Likelihood: Uses these statistical methods to compute the probability of a sentence belonging to a specific label.

Financial Dataset: Uses labeled financial data for training and testing. The classifier is trained to recognize sentences **related to:**

  -Stocks: Sentences related to stock market activity.

  -Cryptocurrency: Sentences related to cryptocurrency trends and news.

  -Commodities: Sentences related to commodity prices and supply.

  -Global Markets: Sentences about global market conditions and economic events.
