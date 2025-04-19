import express from 'express';
import bodyParser from 'body-parser';

const app = express();

// Middleware to parse JSON request bodies
app.use(bodyParser.json());

// Define an endpoint to handle input
app.post('/process-data', (req, res) => {
    const inputData = req.body.input; // Get input from the frontend
    console.log('Received input:', inputData);

    // Process the input (e.g., call your backend function)
    const outputData = `Processed: ${inputData}`;

    // Send the output back to the frontend
    res.json({ output: outputData });
});

app.listen(8080, () => {
    console.log('Server is listening on port 8080');
});
