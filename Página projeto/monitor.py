from flask import Flask, render_template, request, jsonify
import socket

app = Flask(__name__)

RPI_PICO_IP = "192.168.18.123"
PORT = 80
@app.route("/", methods=["GET"])
def index():
    aparelho = request.args.get("aparelho") # Qual aparelho é (ventilador, ar-condicionado, computador...)
    estado = request.args.get("estado")  # "ligar" ou "desligar"

    if not aparelho or not estado:
        return render_template("index.html")
        
    if aparelho and estado:
        print(jsonify({"mensagem": f"Comando recebido: {aparelho} {estado}"}))
        print(f"Comando recebido: {aparelho} - {estado}")
        comando = f"{aparelho}:{estado}"
        print(comando)
        try:
            print("IP - PORTA: ", RPI_PICO_IP, PORT)
        # Conectar ao socket da Pico
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(5) # Para evitar travamentos
                s.connect((RPI_PICO_IP, PORT))
                s.sendall(comando.encode())
                resposta = s.recv(1024).decode()
                print(f"Resposta da Pico: {resposta}")
        
            return jsonify({"mensagem": resposta})
        except Exception as e:
            print(f"Erro na comunicação com a Pico: {e}")
            return jsonify({"erro": str(e)}), 500
    else:
        print("Nenhum comando recebido!")
    return render_template("index.html")

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
