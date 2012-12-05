from flask import Flask, render_template, redirect, request
import motor

app = Flask(__name__)
m = motor.Motor()
print 'OK!'

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/rotate')
def rotate():
	m.rotate()
	return redirect('/')

@app.route('/rotate_stop')
def rotate_stop():
	m.rotate_stop()
	return redirect('/')

@app.route('/rotate_time')
def rotate_time():
	time = request.args.get('time', 0)

	if time:
		m.rotate_time(time)
	return redirect('/')

@app.route('/rotate_cw')
def rotate_cw():
	m.rotate_cw()
	return redirect('/')

@app.route('/rotate_ccw')
def rotate_ccw():
	m.rotate_ccw()
	return redirect('/')

if __name__ == "__main__":
    app.debug = True
    app.run(host='0.0.0.0')
