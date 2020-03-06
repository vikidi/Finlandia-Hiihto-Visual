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
    $('#warning').text('');
    $('#info').text('');
    $('#results').empty();
    $('#fileLabel').html('Choose file');

    // Load the ready model
    neuralNetwork.load(files, finishedTraining)
        .catch(() => {
            $('#warning').text('Error creating model, remember to choose all 3 files');
            $('#button').prop('disabled', true);
            $('#place').prop('disabled', true);
            $('#rain').prop('disabled', true);
            $('#snow').prop('disabled', true);
            $('#temp').prop('disabled', true);
        });
}

// when it is done training
function finishedTraining() {
    // Enable predict button
    $('#button').prop('disabled', false);
    $('#place').prop('disabled', false);
    $('#rain').prop('disabled', false);
    $('#snow').prop('disabled', false);
    $('#temp').prop('disabled', false);

    $('#fileLabel').html('Model loaded');
}

function getResult() {

    $('#results').empty('');
    $('#warning').text('');
    $('#info').text('');

    var place = $('#place').val();
    var rain = $('#rain').val();
    var snow = $('#snow').val();
    var temp = $('#temp').val();

    if (!((place.length > 0) && (rain.length > 0) && (snow.length > 0) && (temp.length > 0))) {
        $('#warning').text('Fill all 4 inputs!');
        return;
    }

    place = parseInt(place);
    rain = parseFloat(rain);
    snow = parseInt(snow);
    temp = parseFloat(temp);

    if (isNaN(place) || isNaN(rain) || isNaN(snow) || isNaN(temp)) {
        $('#warning').text('Check that inputs are valid!');
        return;
    }

    if (place < 1) {
        $('#warning').text('Place needs to be >= 1!');
        return;
    }

    if (snow < 0) {
        $('#warning').text('Snow needs to be >= 0!');
        return;
    }

    neuralNetwork.classify([place, rain, snow, temp], (err, results) => {

        var text = `
        <table class="table table-hover table-sm" style="margin-bottom: 0;">
            <thead>
                <tr>
                <th scope="col">Name</th>
                <th scope="col">Confidence</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                <td>${ results[0].label }</td>
                <td>${ (results[0].confidence * 100).toFixed(2) } %</td>
                </tr>
                <tr>
                <td>${ results[1].label }</td>
                <td>${ (results[1].confidence * 100).toFixed(2) } %</td>
                </tr>
                <tr>
                <td>${ results[2].label }</td>
                <td>${ (results[2].confidence * 100).toFixed(2) } %</td>
                </tr>
                <tr>
                <td>${ results[3].label }</td>
                <td>${ (results[3].confidence * 100).toFixed(2) } %</td>
                </tr>
                <tr>
                <td>${ results[4].label }</td>
                <td>${ (results[4].confidence * 100).toFixed(2) } %</td>
                </tr>
            </tbody>
        </table>
        `;

        $('#results').html(text);
    });
}