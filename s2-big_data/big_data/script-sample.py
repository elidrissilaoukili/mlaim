import tweepy
import json
import os
from datetime import datetime

# Set up client
client = tweepy.Client(bearer_token="AAAAAAAAAAAAAAAAAAAAACQk1QEAAAAAcpprJXffIclPI1bcNLRsuP6GcjQ%3DiynS6r6u8cu9GxJkLWpciwOGmNwgHwnxfN3xkCNCIo169ev10z")

# Get user timeline
user_id = client.get_user(username="AbujomaaGaza").data.id
tweets = client.get_users_tweets(
    id=user_id,
    max_results=10,
    tweet_fields=["created_at", "public_metrics"]
)

# Process tweets
results = []
for tweet in tweets.data:
    tweet_data = {
        "id": tweet.id,
        "text": tweet.text,
        "date": tweet.created_at.isoformat(),
        "metrics": tweet.public_metrics
    }
    results.append(tweet_data)

# Save to file
with open("tweets.json", "w") as f:
    json.dump(results, f, indent=4)