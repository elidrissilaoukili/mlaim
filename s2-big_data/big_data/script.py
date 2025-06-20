#!/usr/bin/env python3
"""
Twitter/X Scraper - A tool to scrape tweets from X (formerly Twitter) and save them in JSON format.
"""

import tweepy
import json
from datetime import datetime
import os
import argparse
import time

class TwitterScraper:
    def __init__(self, consumer_key, consumer_secret, access_token, access_token_secret):
        """Initialize the TwitterScraper with API credentials."""
        auth = tweepy.OAuth1UserHandler(
            consumer_key, consumer_secret, access_token, access_token_secret
        )
        self.api = tweepy.API(auth, wait_on_rate_limit=True)
        self.output_dir = "tweets_output"
        
        # Create output directory if it doesn't exist
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
    
    def scrape_user_tweets(self, username, count=100):
        """Scrape tweets from a specific user."""
        try:
            tweets = self.api.user_timeline(
                screen_name=username,
                count=count,
                tweet_mode="extended"  # Get full text of tweets
            )
            
            results = []
            for tweet in tweets:
                tweet_data = {
                    "id": tweet.id_str,
                    "user": {
                        "id": tweet.user.id_str,
                        "name": tweet.user.name,
                        "screen_name": tweet.user.screen_name,
                        "followers_count": tweet.user.followers_count,
                        "profile_image_url": tweet.user.profile_image_url
                    },
                    "date": tweet.created_at.isoformat(),
                    "text": tweet.full_text,
                    "retweet_count": tweet.retweet_count,
                    "favorite_count": tweet.favorite_count,
                }
                
                # Include media info if available
                if hasattr(tweet, "entities") and "media" in tweet.entities:
                    tweet_data["media"] = []
                    for media in tweet.entities["media"]:
                        tweet_data["media"].append({
                            "type": media["type"],
                            "url": media["media_url_https"]
                        })
                
                results.append(tweet_data)
            
            # Save to JSON file
            filename = f"{self.output_dir}/{username}_tweets_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
            with open(filename, 'w', encoding='utf-8') as f:
                json.dump(results, f, ensure_ascii=False, indent=4)
                
            print(f"Successfully scraped {len(results)} tweets from @{username}")
            print(f"Saved to {filename}")
            return results
            
        except tweepy.TweepyException as e:
            print(f"Error scraping tweets from {username}: {e}")
            return []
    
    def scrape_hashtag(self, hashtag, count=100):
        """Scrape tweets containing a specific hashtag."""
        try:
            query = f"#{hashtag} -filter:retweets"
            tweets = self.api.search_tweets(
                q=query,
                count=count,
                tweet_mode="extended",
                result_type="recent"
            )
            
            results = []
            for tweet in tweets:
                tweet_data = {
                    "id": tweet.id_str,
                    "user": {
                        "id": tweet.user.id_str,
                        "name": tweet.user.name,
                        "screen_name": tweet.user.screen_name,
                        "followers_count": tweet.user.followers_count,
                        "profile_image_url": tweet.user.profile_image_url
                    },
                    "date": tweet.created_at.isoformat(),
                    "text": tweet.full_text,
                    "retweet_count": tweet.retweet_count,
                    "favorite_count": tweet.favorite_count,
                }
                
                # Include media info if available
                if hasattr(tweet, "entities") and "media" in tweet.entities:
                    tweet_data["media"] = []
                    for media in tweet.entities["media"]:
                        tweet_data["media"].append({
                            "type": media["type"],
                            "url": media["media_url_https"]
                        })
                
                results.append(tweet_data)
            
            # Save to JSON file
            filename = f"{self.output_dir}/hashtag_{hashtag}_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
            with open(filename, 'w', encoding='utf-8') as f:
                json.dump(results, f, ensure_ascii=False, indent=4)
                
            print(f"Successfully scraped {len(results)} tweets with hashtag #{hashtag}")
            print(f"Saved to {filename}")
            return results
            
        except tweepy.TweepyException as e:
            print(f"Error scraping tweets with hashtag #{hashtag}: {e}")
            return []
    
    def scrape_search(self, query, count=100):
        """Scrape tweets matching a search query."""
        try:
            tweets = self.api.search_tweets(
                q=query,
                count=count,
                tweet_mode="extended",
                result_type="mixed"
            )
            
            results = []
            for tweet in tweets:
                tweet_data = {
                    "id": tweet.id_str,
                    "user": {
                        "id": tweet.user.id_str,
                        "name": tweet.user.name,
                        "screen_name": tweet.user.screen_name,
                        "followers_count": tweet.user.followers_count,
                        "profile_image_url": tweet.user.profile_image_url
                    },
                    "date": tweet.created_at.isoformat(),
                    "text": tweet.full_text,
                    "retweet_count": tweet.retweet_count,
                    "favorite_count": tweet.favorite_count,
                }
                
                # Include media info if available
                if hasattr(tweet, "entities") and "media" in tweet.entities:
                    tweet_data["media"] = []
                    for media in tweet.entities["media"]:
                        tweet_data["media"].append({
                            "type": media["type"],
                            "url": media["media_url_https"]
                        })
                
                results.append(tweet_data)
            
            # Save to JSON file
            safe_query = query.replace(' ', '_')[:30]  # Create a safe filename
            filename = f"{self.output_dir}/search_{safe_query}_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
            with open(filename, 'w', encoding='utf-8') as f:
                json.dump(results, f, ensure_ascii=False, indent=4)
                
            print(f"Successfully scraped {len(results)} tweets matching query: {query}")
            print(f"Saved to {filename}")
            return results
            
        except tweepy.TweepyException as e:
            print(f"Error scraping tweets with query '{query}': {e}")
            return []


def main():
    parser = argparse.ArgumentParser(description='Scrape tweets from Twitter/X')
    parser.add_argument('--user', help='Twitter username to scrape tweets from')
    parser.add_argument('--hashtag', help='Hashtag to scrape (without the # symbol)')
    parser.add_argument('--query', help='Search query to scrape tweets for')
    parser.add_argument('--count', type=int, default=100, help='Number of tweets to scrape (default: 100)')
    parser.add_argument('--config', default='config.json', help='Path to config file with API keys')
    
    args = parser.parse_args()
    
    # Load API credentials from config file
    try:
        with open(args.config, 'r') as f:
            config = json.load(f)
            consumer_key = config.get('consumer_key')
            consumer_secret = config.get('consumer_secret')
            access_token = config.get('access_token')
            access_token_secret = config.get('access_token_secret')
    except FileNotFoundError:
        print(f"Config file {args.config} not found. Please create it with your Twitter API credentials.")
        print("Example config.json format:")
        print("""
        {
            "consumer_key": "7Xg2VL3oJZe0rTlZgwCZcUYnM",
            "consumer_secret": "PqQKBl1DVzpUrUaiwWWiwIoKsmMtwKG5nTBQT7Yyj44NdQQa7x",
            "access_token": "1922672027141648384-fpAm4gFW7qi7dYl7ILDpw1wRmcrldc",
            "access_token_secret": "4oiblnEv1zzeS57WC9XploaPyNkk5CcGk8f5PIHn9tUFq"
        }
        """)
        return
    
    scraper = TwitterScraper(consumer_key, consumer_secret, access_token, access_token_secret)
    
    if not (args.user or args.hashtag or args.query):
        print("Please specify at least one of: --user, --hashtag, or --query")
        return
    
    if args.user:
        scraper.scrape_user_tweets(args.user, args.count)
    
    if args.hashtag:
        scraper.scrape_hashtag(args.hashtag, args.count)
    
    if args.query:
        scraper.scrape_search(args.query, args.count)


if __name__ == "__main__":
    main()