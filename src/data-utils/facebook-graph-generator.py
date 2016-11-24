"""facebook-graph-generator.py: Uses the Facebook Graph API to generate a graph of mutual friendships"""

import ConfigParser
import requests

# load config
config.read('conf/facebook_graph.ini')
config = ConfigParser.RawConfigParser()

if !config.has_option('Facebook API', 'access_token'):
	raise NameError('access_token not found')
access_token = config.get('Facebook API', 'access_token')

if !config.has_option('Facebook API', 'user_id'):
	raise NameError('user_id not found')
root_id = config.get('Facebook API', 'user_id')

# get friends of user_id
def get_friends(user_id):
	result = {}

	r = requests.get('https://graph.facebook.com/v2.8/%s/friends?access_token=%s'
			% (user_id, access_token))

	if r.status_code != 200:
		raise IOError('API Failure', r.text)

	req_friends = r.json()['data']

	