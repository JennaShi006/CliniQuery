import React, { useState } from 'react';
// Example function to send input to the backend
async function sendDataToBackend(input) {
    try {
        const response = await fetch('http://localhost:8080/process-data', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ input }), // Send input as JSON
        });

        const data = await response.json(); // Parse the JSON response
        console.log('Backend response:', data.output); // Use the backend's output
    } catch (error) {
        console.error('Error communicating with backend:', error);
    }
}

// Example usage
sendDataToBackend('Hello, backend!');