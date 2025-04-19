import React, { useState } from 'react';
// Example function to send input to the backend
//these are not the actual functions to use
async function sendDataToBackend(input) {
    try {
        const response = await fetch('http://localhost:8080/api/data', {
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

async function fetchDataFromCppBackend() {
    try {
        const response = await fetch('http://localhost:8080/api/data', {
            method: 'GET',
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const data = await response.json();
        console.log('Data from C++ backend:', data);
        return data; // Return the data for further processing

        // Use the data in your frontend (e.g., update the UI)
    } catch (error) {
        console.error('Error fetching data from C++ backend:', error);
    }
}

// Example usage
sendDataToBackend('Hello, backend!');

// Call the function to fetch data
fetchDataFromCppBackend();

export {sendDataToBackend, fetchDataFromCppBackend};