import json
import time
import random
import re
import os
from datetime import datetime
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.chrome.options import Options
from selenium.common.exceptions import TimeoutException, NoSuchElementException
import platform

class TwitterScraper:
    def __init__(self, headless=False):
        """Initialize the Twitter scraper with Selenium WebDriver for Brave"""
        self.setup_driver(headless)
        self.scraped_tweets = {}
        
    def get_brave_path(self):
        """Get the Brave browser executable path based on the operating system"""
        system = platform.system()
        
        if system == "Windows":
            # Common Brave paths on Windows
            paths = [
                r"C:\Program Files\BraveSoftware\Brave-Browser\Application\brave.exe",
                r"C:\Program Files (x86)\BraveSoftware\Brave-Browser\Application\brave.exe",
                r"C:\Users\{}\AppData\Local\BraveSoftware\Brave-Browser\Application\brave.exe".format(os.getenv('USERNAME'))
            ]
        elif system == "Darwin":  # macOS
            paths = [
                "/Applications/Brave Browser.app/Contents/MacOS/Brave Browser"
            ]
        else:  # Linux
            paths = [
                "/usr/bin/brave-browser",
                "/usr/bin/brave",
                "/opt/brave.com/brave/brave-browser"
            ]
        
        # Try to find existing Brave installation
        for path in paths:
            if os.path.exists(path):
                return path
        
        # If not found, return the most common path for the system
        if system == "Windows":
            return r"C:\Program Files\BraveSoftware\Brave-Browser\Application\brave.exe"
        elif system == "Darwin":
            return "/Applications/Brave Browser.app/Contents/MacOS/Brave Browser"
        else:
            return "/usr/bin/brave-browser"
        
    def setup_driver(self, headless):
        """Set up Brave WebDriver with appropriate options"""
        chrome_options = Options()
        
        # Set Brave browser path
        brave_path = self.get_brave_path()
        chrome_options.binary_location = brave_path
        
        if headless:
            chrome_options.add_argument('--headless')
        
        # Use existing user data directory to maintain login session
        # This will use your existing Brave profile with saved logins
        system = platform.system()
        if system == "Windows":
            user_data_dir = r"C:\Users\{}\AppData\Local\BraveSoftware\Brave-Browser\User Data".format(os.getenv('USERNAME'))
        elif system == "Darwin":  # macOS
            user_data_dir = os.path.expanduser("~/Library/Application Support/BraveSoftware/Brave-Browser")
        else:  # Linux
            user_data_dir = os.path.expanduser("~/.config/BraveSoftware/Brave-Browser")
        
        chrome_options.add_argument(f'--user-data-dir={user_data_dir}')
        chrome_options.add_argument('--profile-directory=Default')  # Use default profile
        
        # Important: These options help avoid detection
        chrome_options.add_argument('--no-sandbox')
        chrome_options.add_argument('--disable-dev-shm-usage')
        chrome_options.add_argument('--disable-blink-features=AutomationControlled')
        chrome_options.add_experimental_option("excludeSwitches", ["enable-automation"])
        chrome_options.add_experimental_option('useAutomationExtension', False)
        
        # Set user agent to appear more like a real browser
        chrome_options.add_argument('--user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36')
        
        # Disable notifications and other potential interruptions
        prefs = {
            "profile.default_content_setting_values.notifications": 2,
            "profile.default_content_settings.popups": 0,
            "profile.managed_default_content_settings.images": 2
        }
        chrome_options.add_experimental_option("prefs", prefs)
        
        try:
            self.driver = webdriver.Chrome(options=chrome_options)
            self.driver.execute_script("Object.defineProperty(navigator, 'webdriver', {get: () => undefined})")
            print("✅ Brave browser opened successfully with existing profile")
        except Exception as e:
            print(f"❌ Error opening Brave browser: {e}")
            print("Make sure:")
            print("1. Brave browser is installed")
            print("2. ChromeDriver is installed and in PATH")
            print("3. ChromeDriver version matches your Brave version")
            raise
        
    def login_check(self):
        """Check if already logged into Twitter/X"""
        print("Checking login status...")
        self.driver.get("https://x.com/home")
        time.sleep(3)
        
        # Check if we're on the login page or already logged in
        current_url = self.driver.current_url
        if "login" in current_url or "i/flow/login" in current_url:
            print("❌ Not logged in. Please log in to Twitter/X manually in the browser.")
            input("Press Enter after you've logged in...")
            return False
        else:
            print("✅ Already logged in to Twitter/X")
            return True
        
    def parse_count(self, text):
        """Parse abbreviated numbers like 1.2K, 1M to actual numbers"""
        if not text or text.strip() == '':
            return 0
            
        text = text.strip().replace(',', '')
        
        try:
            if 'K' in text:
                return int(float(text.replace('K', '')) * 1000)
            elif 'M' in text:
                return int(float(text.replace('M', '')) * 1000000)
            elif 'B' in text:
                return int(float(text.replace('B', '')) * 1000000000)
            else:
                return int(text)
        except (ValueError, TypeError):
            return 0
    
    def extract_tweet_data(self, tweet_element):
        """Extract data from a single tweet element"""
        try:
            # Extract tweet text
            tweet_text = ""
            try:
                text_element = tweet_element.find_element(By.CSS_SELECTOR, '[data-testid="tweetText"]')
                tweet_text = text_element.text
            except NoSuchElementException:
                pass
            
            # Extract username
            username = ""
            try:
                user_element = tweet_element.find_element(By.CSS_SELECTOR, '[data-testid="User-Name"] a')
                username = user_element.get_attribute('href').split('/')[-1] if user_element.get_attribute('href') else ""
            except NoSuchElementException:
                pass
            
            # Extract timestamp and tweet URL
            tweet_url = ""
            tweet_date = ""
            try:
                time_element = tweet_element.find_element(By.CSS_SELECTOR, 'time')
                tweet_date = time_element.get_attribute('datetime')
                parent_link = time_element.find_element(By.XPATH, './..')
                tweet_url = parent_link.get_attribute('href')
            except NoSuchElementException:
                pass
            
            # Extract tweet ID from URL
            tweet_id = ""
            if tweet_url:
                tweet_id = tweet_url.split('/')[-1]
            
            # Extract metrics
            metrics = {
                "reply_count": 0,
                "retweet_count": 0,
                "like_count": 0,
                "quote_count": 0,
                "bookmark_count": 0,
                "impression_count": 0
            }
            
            # Reply count
            try:
                reply_element = tweet_element.find_element(By.CSS_SELECTOR, '[data-testid="reply"]')
                reply_text = reply_element.find_element(By.CSS_SELECTOR, 'span').text
                metrics["reply_count"] = self.parse_count(reply_text)
            except NoSuchElementException:
                pass
            
            # Retweet count
            try:
                retweet_element = tweet_element.find_element(By.CSS_SELECTOR, '[data-testid="retweet"]')
                retweet_text = retweet_element.find_element(By.CSS_SELECTOR, 'span').text
                metrics["retweet_count"] = self.parse_count(retweet_text)
            except NoSuchElementException:
                pass
            
            # Like count
            try:
                like_element = tweet_element.find_element(By.CSS_SELECTOR, '[data-testid="like"]')
                like_text = like_element.find_element(By.CSS_SELECTOR, 'span').text
                metrics["like_count"] = self.parse_count(like_text)
            except NoSuchElementException:
                pass
            
            # View count (impression count)
            try:
                view_element = tweet_element.find_element(By.CSS_SELECTOR, 'a[href*="/analytics"]')
                view_text = view_element.find_element(By.CSS_SELECTOR, 'span').text
                metrics["impression_count"] = self.parse_count(view_text)
            except NoSuchElementException:
                pass
            
            return {
                "id": int(tweet_id) if tweet_id.isdigit() else hash(tweet_text + username),
                "text": tweet_text,
                "date": tweet_date or datetime.now().isoformat(),
                "user": username,
                "url": tweet_url,
                "metrics": metrics
            }
            
        except Exception as e:
            print(f"Error extracting tweet data: {e}")
            return None
    
    def scrape_profile(self, profile_url, max_tweets=300, scroll_pause=2):
        """Scrape tweets from a specific profile"""
        print(f"Scraping tweets from: {profile_url}")
        
        self.driver.get(profile_url)
        time.sleep(random.uniform(3, 5))
        
        tweets_scraped = 0
        last_height = self.driver.execute_script("return document.body.scrollHeight")
        consecutive_no_new_tweets = 0
        
        while tweets_scraped < max_tweets:
            # Find all tweet elements (don't limit to 5, that's causing the issue)
            tweet_elements = self.driver.find_elements(By.CSS_SELECTOR, 'article[data-testid="tweet"]')
            
            tweets_found_this_iteration = 0
            
            # Process new tweets
            for tweet_element in tweet_elements:
                tweet_data = self.extract_tweet_data(tweet_element)
                
                if tweet_data and tweet_data['id'] not in self.scraped_tweets:
                    self.scraped_tweets[tweet_data['id']] = tweet_data
                    tweets_scraped += 1
                    tweets_found_this_iteration += 1
                    print(f"Scraped tweet {tweets_scraped}: {tweet_data['text'][:50]}...")
                    
                    if tweets_scraped >= max_tweets:
                        break
                        
                    # Add longer pause every 50 tweets to avoid rate limiting
                    if tweets_scraped % 50 == 0:
                        print(f"Pausing for rate limiting... (scraped {tweets_scraped} tweets)")
                        time.sleep(random.uniform(30, 60))
            
            # If no new tweets found, increment counter
            if tweets_found_this_iteration == 0:
                consecutive_no_new_tweets += 1
            else:
                consecutive_no_new_tweets = 0
                
            # If we haven't found new tweets for 3 consecutive iterations, we might be at the bottom
            if consecutive_no_new_tweets >= 3:
                print("No new tweets found for 3 consecutive iterations, checking if at bottom...")
                
            # Scroll down to load more tweets
            self.driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
            time.sleep(random.uniform(2, 4))
            
            # Check if we've reached the bottom
            new_height = self.driver.execute_script("return document.body.scrollHeight")
            if new_height == last_height:
                consecutive_no_new_tweets += 1
                if consecutive_no_new_tweets >= 5:
                    print("Reached the bottom of the page")
                    break
            else:
                consecutive_no_new_tweets = 0
                
            last_height = new_height
        
        return list(self.scraped_tweets.values())
    
    def save_to_json(self, filename=None):
        """Save scraped tweets to JSON file"""
        if not filename:
            filename = f"tweets_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
        
        with open(filename, 'w', encoding='utf-8') as f:
            json.dump(list(self.scraped_tweets.values()), f, indent=2, ensure_ascii=False)
        
        print(f"Saved {len(self.scraped_tweets)} tweets to {filename}")
        return filename
    
    def close(self):
        """Close the WebDriver"""
        self.driver.quit()

# Usage example
def main():
    # Initialize scraper
    scraper = TwitterScraper(headless=False)  # Set to True for headless mode
    
    try:
        # Check if already logged in (should be automatic with existing profile)
        scraper.login_check()
        
        # Scrape tweets from a profile
        username = input('Enter username: ')
        max_tweets_number = input('Enter number of tweets: ')
        profile_url = f"https://x.com/{username}"  # Replace with desired profile
        tweets = scraper.scrape_profile(profile_url, max_tweets=int(max_tweets_number))
        
        # Print results
        print(f"\nScraped {len(tweets)} tweets:")
        for tweet in tweets[:3]:  # Show first 3 tweets
            print(json.dumps(tweet, indent=2))
        
        # Save to file
        scraper.save_to_json()
        
    except Exception as e:
        print(f"Error: {e}")
    finally:
        scraper.close()

if __name__ == "__main__":
    # Install required packages first:
    # pip install selenium webdriver-manager
    
    print("Twitter/X Scraper for Brave Browser")
    print("=" * 50)
    print("✅ Uses your existing Brave profile and login session")
    print("⚠️  IMPORTANT DISCLAIMERS:")
    print("1. This may violate Twitter's Terms of Service")
    print("2. Use responsibly and respect rate limits")
    print("3. Consider using official Twitter API instead")
    print("4. Make sure you're logged into Twitter/X in Brave")
    print("=" * 50)
    
    main()