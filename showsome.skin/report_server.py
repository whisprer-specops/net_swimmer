from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/', methods=['POST'])
def receive():
    data = request.get_json()
    print("[📨] Got ping from agent:", data)
    return jsonify({"status": "received"}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=443, ssl_context=('fullchain.pem', 'privkey.pem'))
