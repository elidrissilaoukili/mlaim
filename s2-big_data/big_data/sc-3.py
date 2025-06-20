#!/usr/bin/env python3
"""
Twitter/X Scraper - A tool to scrape tweets from X (formerly Twitter) and save them in JSON format.
Uses Twitter API v2 with bearer token authentication (works with free tier).
"""

import tweepy
import json
from datetime import datetime
import os
import argparse
import time

class TwitterScraper:
    def __init__(self, bearer_token):
        """Initialize the TwitterScraper with API credentials."""
        self.client = tweepy.Client(bearer_token=bearer_token, wait_on_rate_limit=True)
        self.output_dir = "tweets_output"
        
        # Create output directory if it doesn't exist
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
    
    def scrape_user_tweets(self, username, count=10):
        """Scrape tweets from a specific user."""
        try:
            # First get the user ID from username
            user_response = self.client.get_user(username=username)
            if not user_response.data:
                print(f"Error: User @{username} not found")
                return []
                
            user_id = user_response.data.id
            user_name = user_response.data.name
            
            # Now get the user's tweets
            tweets_response = self.client.get_users_tweets(
                id=user_id,
                max_results=min(count, 100),  # API v2 max is 100 per request
                tweet_fields=["created_at", "public_metrics", "entities", "attachments"],
                expansions=["attachments.media_keys"],
                media_fields=["type", "url", "preview_image_url"]
            )
            
            if not tweets_response.data:
                print(f"No tweets found for @{username}")
                return []
            
            results = []
            for tweet in tweets_response.data:
                tweet_data = {
                    "id": str(tweet.id),
                    "user": {
                        "id": str(user_id),
                        "name": user_name,
                        "screen_name": username
                    },
                    "date": tweet.created_at.isoformat(),
                    "text": tweet.text,
                }
                
                # Add metrics if available
                if hasattr(tweet, "public_metrics"):
                    tweet_data["retweet_count"] = tweet.public_metrics.get("retweet_count", 0)
                    tweet_data["like_count"] = tweet.public_metrics.get("like_count", 0)
                    tweet_data["reply_count"] = tweet.public_metrics.get("reply_count", 0)
                    tweet_data["quote_count"] = tweet.public_metrics.get("quote_count", 0)
                
                # Add media if available
                if hasattr(tweet, "attachments") and hasattr(tweet.attachments, "media_keys") and tweets_response.includes:
                    media_lookup = {media.media_key: media for media in tweets_response.includes.get("media", [])}
                    tweet_data["media"] = []
                    
                    for media_key in tweet.attachments.media_keys:
                        if media_key in media_lookup:
                            media = media_lookup[media_key]
                            media_info = {
                                "type": media.type
                            }
                            if hasattr(media, "url") and media.url:
                                media_info["url"] = media.url
                            elif hasattr(media, "preview_image_url") and media.preview_image_url:
                                media_info["url"] = media.preview_image_url
                                
                            tweet_data["media"].append(media_info)
                
                results.append(tweet_data)
            
            # If we need more tweets and pagination is available
            if count > 100 and tweets_response.meta.get("next_token"):
                pagination_token = tweets_response.meta["next_token"]
                remaining = count - 100
                
                while pagination_token and remaining > 0:
                    next_page = self.client.get_users_tweets(
                        id=user_id,
                        max_results=min(remaining, 100),
                        pagination_token=pagination_token,
                        tweet_fields=["created_at", "public_metrics", "entities", "attachments"],
                        expansions=["attachments.media_keys"],
                        media_fields=["type", "url", "preview_image_url"]
                    )
                    
                    if not next_page.data:
                        break
                        
                    for tweet in next_page.data:
                        tweet_data = {
                            "id": str(tweet.id),
                            "user": {
                                "id": str(user_id),
                                "name": user_name,
                                "screen_name": username
                            },
                            "date": tweet.created_at.isoformat(),
                            "text": tweet.text,
                        }
                        
                        # Add metrics if available
                        if hasattr(tweet, "public_metrics"):
                            tweet_data["retweet_count"] = tweet.public_metrics.get("retweet_count", 0)
                            tweet_data["like_count"] = tweet.public_metrics.get("like_count", 0)
                            tweet_data["reply_count"] = tweet.public_metrics.get("reply_count", 0)
                            tweet_data["quote_count"] = tweet.public_metrics.get("quote_count", 0)
                        
                        # Add media if available
                        if hasattr(tweet, "attachments") and hasattr(tweet.attachments, "media_keys") and next_page.includes:
                            media_lookup = {media.media_key: media for media in next_page.includes.get("media", [])}
                            tweet_data["media"] = []
                            
                            for media_key in tweet.attachments.media_keys:
                                if media_key in media_lookup:
                                    media = media_lookup[media_key]
                                    media_info = {
                                        "type": media.type
                                    }
                                    if hasattr(media, "url") and media.url:
                                        media_info["url"] = media.url
                                    elif hasattr(media, "preview_image_url") and media.preview_image_url:
                                        media_info["url"] = media.preview_image_url
                                        
                                    tweet_data["media"].append(media_info)
                        
                        results.append(tweet_data)
                    
                    remaining -= len(next_page.data)
                    if next_page.meta.get("next_token"):
                        pagination_token = next_page.meta["next_token"]
                    else:
                        break
            
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
    
    def scrape_hashtag(self, hashtag, count=10):
        """Scrape tweets containing a specific hashtag."""
        try:
            query = f"#{hashtag} -is:retweet"
            
            search_response = self.client.search_recent_tweets(
                query=query,
                max_results=min(count, 100),
                tweet_fields=["created_at", "public_metrics", "entities", "attachments", "author_id"],
                expansions=["author_id", "attachments.media_keys"],
                media_fields=["type", "url", "preview_image_url"],
                user_fields=["name", "username"]
            )
            
            if not search_response.data:
                print(f"No recent tweets found with hashtag #{hashtag}")
                return []
                
            results = []
            
            # Create lookup tables for efficient access
            users = {user.id: user for user in search_response.includes.get("users", [])}
            media = {media.media_key: media for media in search_response.includes.get("media", [])} if search_response.includes else {}
            
            for tweet in search_response.data:
                # Get user info
                user = users.get(tweet.author_id)
                
                tweet_data = {
                    "id": str(tweet.id),
                    "user": {
                        "id": str(tweet.author_id),
                        "name": user.name if user else "Unknown",
                        "screen_name": user.username if user else "unknown"
                    },
                    "date": tweet.created_at.isoformat(),
                    "text": tweet.text,
                }
                
                # Add metrics if available
                if hasattr(tweet, "public_metrics"):
                    tweet_data["retweet_count"] = tweet.public_metrics.get("retweet_count", 0)
                    tweet_data["like_count"] = tweet.public_metrics.get("like_count", 0)
                    tweet_data["reply_count"] = tweet.public_metrics.get("reply_count", 0)
                    tweet_data["quote_count"] = tweet.public_metrics.get("quote_count", 0)
                
                # Add media if available
                if hasattr(tweet, "attachments") and hasattr(tweet.attachments, "media_keys"):
                    tweet_data["media"] = []
                    
                    for media_key in tweet.attachments.media_keys:
                        if media_key in media:
                            media_item = media[media_key]
                            media_info = {
                                "type": media_item.type
                            }
                            if hasattr(media_item, "url") and media_item.url:
                                media_info["url"] = media_item.url
                            elif hasattr(media_item, "preview_image_url") and media_item.preview_image_url:
                                media_info["url"] = media_item.preview_image_url
                                
                            tweet_data["media"].append(media_info)
                
                results.append(tweet_data)
            
            # If we need more tweets and pagination is available
            if count > 100 and search_response.meta.get("next_token"):
                pagination_token = search_response.meta["next_token"]
                remaining = count - 100
                
                while pagination_token and remaining > 0:
                    next_page = self.client.search_recent_tweets(
                        query=query,
                        max_results=min(remaining, 100),
                        pagination_token=pagination_token,
                        tweet_fields=["created_at", "public_metrics", "entities", "attachments", "author_id"],
                        expansions=["author_id", "attachments.media_keys"],
                        media_fields=["type", "url", "preview_image_url"],
                        user_fields=["name", "username"]
                    )
                    
                    if not next_page.data:
                        break
                        
                    # Update lookup tables
                    users.update({user.id: user for user in next_page.includes.get("users", [])})
                    if next_page.includes and "media" in next_page.includes:
                        media.update({m.media_key: m for m in next_page.includes.get("media", [])})
                    
                    for tweet in next_page.data:
                        # Get user info
                        user = users.get(tweet.author_id)
                        
                        tweet_data = {
                            "id": str(tweet.id),
                            "user": {
                                "id": str(tweet.author_id),
                                "name": user.name if user else "Unknown",
                                "screen_name": user.username if user else "unknown"
                            },
                            "date": tweet.created_at.isoformat(),
                            "text": tweet.text,
                        }
                        
                        # Add metrics if available
                        if hasattr(tweet, "public_metrics"):
                            tweet_data["retweet_count"] = tweet.public_metrics.get("retweet_count", 0)
                            tweet_data["like_count"] = tweet.public_metrics.get("like_count", 0)
                            tweet_data["reply_count"] = tweet.public_metrics.get("reply_count", 0)
                            tweet_data["quote_count"] = tweet.public_metrics.get("quote_count", 0)
                        
                        # Add media if available
                        if hasattr(tweet, "attachments") and hasattr(tweet.attachments, "media_keys"):
                            tweet_data["media"] = []
                            
                            for media_key in tweet.attachments.media_keys:
                                if media_key in media:
                                    media_item = media[media_key]
                                    media_info = {
                                        "type": media_item.type
                                    }
                                    if hasattr(media_item, "url") and media_item.url:
                                        media_info["url"] = media_item.url
                                    elif hasattr(media_item, "preview_image_url") and media_item.preview_image_url:
                                        media_info["url"] = media_item.preview_image_url
                                        
                                    tweet_data["media"].append(media_info)
                        
                        results.append(tweet_data)
                    
                    remaining -= len(next_page.data)
                    if next_page.meta.get("next_token"):
                        pagination_token = next_page.meta["next_token"]
                    else:
                        break
            
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
    
    def scrape_search(self, query, count=10):
        """Scrape tweets matching a search query."""
        try:
            search_query = f"{query} -is:retweet"
            
            search_response = self.client.search_recent_tweets(
                query=search_query,
                max_results=min(count, 100),
                tweet_fields=["created_at", "public_metrics", "entities", "attachments", "author_id"],
                expansions=["author_id", "attachments.media_keys"],
                media_fields=["type", "url", "preview_image_url"],
                user_fields=["name", "username"]
            )
            
            if not search_response.data:
                print(f"No recent tweets found matching query: {query}")
                return []
                
            results = []
            
            # Create lookup tables for efficient access
            users = {user.id: user for user in search_response.includes.get("users", [])}
            media = {media.media_key: media for media in search_response.includes.get("media", [])} if search_response.includes and "media" in search_response.includes else {}
            
            for tweet in search_response.data:
                # Get user info
                user = users.get(tweet.author_id)
                
                tweet_data = {
                    "id": str(tweet.id),
                    "user": {
                        "id": str(tweet.author_id),
                        "name": user.name if user else "Unknown",
                        "screen_name": user.username if user else "unknown"
                    },
                    "date": tweet.created_at.isoformat(),
                    "text": tweet.text,
                }
                
                # Add metrics if available
                if hasattr(tweet, "public_metrics"):
                    tweet_data["retweet_count"] = tweet.public_metrics.get("retweet_count", 0)
                    tweet_data["like_count"] = tweet.public_metrics.get("like_count", 0)
                    tweet_data["reply_count"] = tweet.public_metrics.get("reply_count", 0)
                    tweet_data["quote_count"] = tweet.public_metrics.get("quote_count", 0)
                
                # Add media if available
                if hasattr(tweet, "attachments") and hasattr(tweet.attachments, "media_keys"):
                    tweet_data["media"] = []
                    
                    for media_key in tweet.attachments.media_keys:
                        if media_key in media:
                            media_item = media[media_key]
                            media_info = {
                                "type": media_item.type
                            }
                            if hasattr(media_item, "url") and media_item.url:
                                media_info["url"] = media_item.url
                            elif hasattr(media_item, "preview_image_url") and media_item.preview_image_url:
                                media_info["url"] = media_item.preview_image_url
                                
                            tweet_data["media"].append(media_info)
                
                results.append(tweet_data)
            
            # If we need more tweets and pagination is available
            if count > 100 and search_response.meta.get("next_token"):
                pagination_token = search_response.meta["next_token"]
                remaining = count - 100
                
                while pagination_token and remaining > 0:
                    next_page = self.client.search_recent_tweets(
                        query=search_query,
                        max_results=min(remaining, 100),
                        pagination_token=pagination_token,
                        tweet_fields=["created_at", "public_metrics", "entities", "attachments", "author_id"],
                        expansions=["author_id", "attachments.media_keys"],
                        media_fields=["type", "url", "preview_image_url"],
                        user_fields=["name", "username"]
                    )
                    
                    if not next_page.data:
                        break
                        
                    # Update lookup tables
                    users.update({user.id: user for user in next_page.includes.get("users", [])})
                    if next_page.includes and "media" in next_page.includes:
                        media.update({m.media_key: m for m in next_page.includes.get("media", [])})
                    
                    for tweet in next_page.data:
                        # Get user info
                        user = users.get(tweet.author_id)
                        
                        tweet_data = {
                            "id": str(tweet.id),
                            "user": {
                                "id": str(tweet.author_id),
                                "name": user.name if user else "Unknown",
                                "screen_name": user.username if user else "unknown"
                            },
                            "date": tweet.created_at.isoformat(),
                            "text": tweet.text,
                        }
                        
                        # Add metrics if available
                        if hasattr(tweet, "public_metrics"):
                            tweet_data["retweet_count"] = tweet.public_metrics.get("retweet_count", 0)
                            tweet_data["like_count"] = tweet.public_metrics.get("like_count", 0)
                            tweet_data["reply_count"] = tweet.public_metrics.get("reply_count", 0)
                            tweet_data["quote_count"] = tweet.public_metrics.get("quote_count", 0)
                        
                        # Add media if available
                        if hasattr(tweet, "attachments") and hasattr(tweet.attachments, "media_keys"):
                            tweet_data["media"] = []
                            
                            for media_key in tweet.attachments.media_keys:
                                if media_key in media:
                                    media_item = media[media_key]
                                    media_info = {
                                        "type": media_item.type
                                    }
                                    if hasattr(media_item, "url") and media_item.url:
                                        media_info["url"] = media_item.url
                                    elif hasattr(media_item, "preview_image_url") and media_item.preview_image_url:
                                        media_info["url"] = media_item.preview_image_url
                                        
                                    tweet_data["media"].append(media_info)
                        
                        results.append(tweet_data)
                    
                    remaining -= len(next_page.data)
                    if next_page.meta.get("next_token"):
                        pagination_token = next_page.meta["next_token"]
                    else:
                        break
            
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
    parser.add_argument('--count', type=int, default=10, help='Number of tweets to scrape (default: 10)')
    parser.add_argument('--config', default='config.json', help='Path to config file with API keys')
    
    args = parser.parse_args()
    
    # Use hardcoded bearer token to make it copy and paste ready
    bearer_token = "YOUR_BEARER_TOKEN_HERE"
    
    # Load bearer token from config file if available
    try:
        with open(args.config, 'r') as f:
            config = json.load(f)
            if 'bearer_token' in config and config['bearer_token']:
                bearer_token = config['bearer_token']
    except FileNotFoundError:
        # If using hardcoded token, don't worry about config file
        if bearer_token == "YOUR_BEARER_TOKEN_HERE":
            print(f"Config file {args.config} not found. Please either:")
            print("1. Create it with your Twitter API bearer token:")
            print("""
            {
                "bearer_token": "YOUR_BEARER_TOKEN"
            }
            """)
            print("2. Or replace 'YOUR_BEARER_TOKEN_HERE' in the script with your actual bearer token")
            return
    
    # If still using placeholder token, exit
    if bearer_token == "YOUR_BEARER_TOKEN_HERE":
        print("Please replace 'YOUR_BEARER_TOKEN_HERE' in the script with your actual Twitter API bearer token")
        print("You can get this from the Twitter Developer Portal: https://developer.twitter.com/")
        return
    
    scraper = TwitterScraper(bearer_token)
    
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