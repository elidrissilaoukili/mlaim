// JavaScript approach - runs in browser console while logged into X
// This script extracts tweet data from the current page

function scrapeTweets() {
    const tweets = [];
    
    // Find all tweet articles on the page
    const tweetElements = document.querySelectorAll('article[data-testid="tweet"]');
    
    tweetElements.forEach((article, index) => {
        try {
            // Extract tweet text
            const tweetTextElement = article.querySelector('[data-testid="tweetText"]');
            const tweetText = tweetTextElement ? tweetTextElement.innerText : '';
            
            // Extract username
            const userNameElement = article.querySelector('[data-testid="User-Name"] a');
            const userName = userNameElement ? userNameElement.getAttribute('href').replace('/', '') : '';
            
            // Extract timestamp and construct tweet URL
            const timeElement = article.querySelector('time');
            const datetime = timeElement ? timeElement.getAttribute('datetime') : '';
            const tweetUrl = timeElement ? timeElement.parentElement.getAttribute('href') : '';
            
            // Extract tweet ID from URL
            const tweetId = tweetUrl ? tweetUrl.split('/').pop() : '';
            
            // Extract metrics (replies, retweets, likes, views)
            const metrics = {};
            
            // Reply count
            const replyElement = article.querySelector('[data-testid="reply"]');
            const replyCount = replyElement ? extractCount(replyElement) : 0;
            
            // Retweet count
            const retweetElement = article.querySelector('[data-testid="retweet"]');
            const retweetCount = retweetElement ? extractCount(retweetElement) : 0;
            
            // Like count
            const likeElement = article.querySelector('[data-testid="like"]');
            const likeCount = likeElement ? extractCount(likeElement) : 0;
            
            // View count (if available)
            const viewElement = article.querySelector('a[href*="/analytics"]');
            const viewCount = viewElement ? extractCount(viewElement) : 0;
            
            // Bookmark count (usually not visible)
            const bookmarkElement = article.querySelector('[data-testid="bookmark"]');
            const bookmarkCount = bookmarkElement ? extractCount(bookmarkElement) : 0;
            
            metrics.reply_count = replyCount;
            metrics.retweet_count = retweetCount;
            metrics.like_count = likeCount;
            metrics.quote_count = 0; // Quote count is harder to extract
            metrics.bookmark_count = bookmarkCount;
            metrics.impression_count = viewCount;
            
            // Create tweet object
            const tweet = {
                id: parseInt(tweetId) || Date.now() + index,
                text: tweetText,
                date: datetime || new Date().toISOString(),
                user: userName,
                url: tweetUrl ? `https://x.com${tweetUrl}` : '',
                metrics: metrics
            };
            
            tweets.push(tweet);
            
        } catch (error) {
            console.error('Error processing tweet:', error);
        }
    });
    
    return tweets;
}

// Helper function to extract count from elements
function extractCount(element) {
    if (!element) return 0;
    
    // Look for count in various possible locations
    const countSelectors = [
        'span[data-testid="app-text-transition-container"] span',
        'span.css-1jxf684',
        'span'
    ];
    
    for (let selector of countSelectors) {
        const countElement = element.querySelector(selector);
        if (countElement && countElement.textContent.trim()) {
            const text = countElement.textContent.trim();
            // Convert abbreviated numbers (1.2K, 1M, etc.) to actual numbers
            return parseCount(text);
        }
    }
    
    return 0;
}

// Helper function to parse abbreviated numbers
function parseCount(text) {
    if (!text || text === '') return 0;
    
    const cleanText = text.replace(/,/g, '');
    const number = parseFloat(cleanText);
    
    if (isNaN(number)) return 0;
    
    if (text.includes('K')) return Math.floor(number * 1000);
    if (text.includes('M')) return Math.floor(number * 1000000);
    if (text.includes('B')) return Math.floor(number * 1000000000);
    
    return Math.floor(number);
}

// Function to continuously scrape as you scroll
function continuousScrape(intervalMs = 5000) {
    const allTweets = new Map();
    
    const scrapeInterval = setInterval(() => {
        const newTweets = scrapeTweets();
        
        newTweets.forEach(tweet => {
            allTweets.set(tweet.id, tweet);
        });
        
        console.log(`Total unique tweets scraped: ${allTweets.size}`);
        
        // Auto-scroll to load more tweets
        window.scrollTo(0, document.body.scrollHeight);
        
    }, intervalMs);
    
    // Return function to stop scraping and get results
    return {
        stop: () => {
            clearInterval(scrapeInterval);
            const results = Array.from(allTweets.values());
            console.log('Final results:', results);
            return results;
        },
        getTweets: () => Array.from(allTweets.values())
    };
}

// Usage examples:
// 1. Scrape current visible tweets:
// const tweets = scrapeTweets();
// console.log(JSON.stringify(tweets, null, 2));

// 2. Continuous scraping with auto-scroll:
// const scraper = continuousScrape();
// // Let it run for a while, then stop:
// // const allTweets = scraper.stop();

// 3. Download results as JSON file:
function downloadTweets(tweets) {
    const dataStr = JSON.stringify(tweets, null, 2);
    const dataUri = 'data:application/json;charset=utf-8,'+ encodeURIComponent(dataStr);
    
    const exportFileDefaultName = `tweets_${new Date().toISOString().split('T')[0]}.json`;
    
    const linkElement = document.createElement('a');
    linkElement.setAttribute('href', dataUri);
    linkElement.setAttribute('download', exportFileDefaultName);
    linkElement.click();
}

// Example usage:
// const tweets = scrapeTweets();
// downloadTweets(tweets);