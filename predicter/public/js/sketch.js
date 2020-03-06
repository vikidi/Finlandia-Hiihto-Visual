const options = {
    inputs: ['place', 'rain', 'snow', 'temp'],
    outputs: ['name'],
    epochs: 250,
    learningRate: 8,
    debug: false,
    task: 'classification'
};

// Initialize the the neural network
const neuralNetwork = ml5.neuralNetwork(options);

function trainModel(files) {
    $('#button').prop('disabled', true);
    $('#warning').text('');
    $('#info').text('');
    $('#results').text('');

    // Load the ready model
    neuralNetwork.load(files, finishedTraining)
        .catch(() => {
            $('#warning').text('Error creating model, remember to choose all 3 files');
        });
}

// when it is done training
function finishedTraining() {
    // Enable predict button
    $('#button').prop('disabled', false);
    $('#info').text('Model loaded!');
}

function getResult() {

    var place = parseInt($('#place').val());
    var rain = parseFloat($('#rain').val());
    var snow = parseInt($('#snow').val());
    var temp = parseFloat($('#temp').val());

    neuralNetwork.classify([place, rain, snow, temp], (err, results) => {
        console.log(results);

        var text = `
        ${results[0].label}: ${ (results[0].confidence * 100).toFixed(2) } % <br>
        ${results[1].label}: ${ (results[1].confidence * 100).toFixed(2) } % <br>
        ${results[2].label}: ${ (results[2].confidence * 100).toFixed(2) } % <br>
        ${results[3].label}: ${ (results[3].confidence * 100).toFixed(2) } % <br>
        ${results[4].label}: ${ (results[4].confidence * 100).toFixed(2) } %
        `;

        $('#results').html(text);
    });
}