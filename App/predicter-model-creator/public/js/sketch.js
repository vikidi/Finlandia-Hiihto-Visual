const options = {
    inputs: ['place', 'rain', 'snow', 'temp'],
    outputs: ['name'],
    epochs: 250,
    learningRate: 8,
    debug: true,
    task: 'classification'
};

// Initialize the the neural network
const neuralNetwork = ml5.neuralNetwork(options);

var topSkiers = {};

function createModel() {

    $('#warning').text('');

    // Get the data 
    var xhr = new XMLHttpRequest();

    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4){
            var data = JSON.parse(this.responseText);
            handleData(data);
        }
    };

    var distance = $('#distance').val();
    var url = new URL(window.location.href + 'data');
    url.searchParams.set('distance', distance);

    xhr.open('GET', url, true);
    xhr.send(null);
}

function handleData(data) {
    $('#results').text('');

    // Needs more data than 500 rows
    if (data.length < 500) {
        $('#warning').text('Not enough data, can\'t use this distance');
        return;
    }

    data.forEach(row => {

        var input = {
            place: parseInt(row.place),
            rain: row.rain,
            snow: row.snow,
            temp: row.temp
        };
    
        var output = {
            name: row.name
        };
    
        neuralNetwork.addData(input, output);
    });

    trainModel();
}

function trainModel() {

    // normalize your data
    neuralNetwork.data.normalize();

    // train your model
    neuralNetwork.train(finishedTraining)
        .catch(() => {
            $('#warning').text('Error creating model');
        });
}

// when it is done training
function finishedTraining() {
    // Save model
    neuralNetwork.save();

    $('#results').text('Done training!');
}