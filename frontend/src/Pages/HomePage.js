import InputBar from "../Components/InputBar";
import FetchResults from "../Components/FetchResults";
import React, { useState } from 'react';

export const HomePage = () => {
    const [dataSubmitted, setDataSubmitted] = useState(false);
    const [input, setInput] = useState(null);
    const handleInputSubmit = async (input) => {
        // Handle the input submission here
        setInput(input); // Store the input for later use
        console.log("Input submitted:", input.value);
        // You can send this input to your backend or process it as needed
        try{
            await fetch('http://localhost:8080/api/data', {
                method: 'POST',
                mode: "no-cors",
                headers: {
                    'Content-Type': 'text/plain', // Set the content type to plain text
                },
                body: input.value, // Send input as JSON
            });

    
            console.log('Request sent to backend'); // Use the backend's output
            setDataSubmitted(true); // Set the state to indicate data has been submitted
        }
        catch (error) {
            console.error('Error communicating with backend:', error);
        }
    };

    // You can also handle the response from the backend here

    return (
        <div>
            <h1>Welcome to CliniQuery</h1>
            <p>This is the main content of the home page.</p>
            <InputBar onSubmit={handleInputSubmit}/>
            {dataSubmitted && (
                <>
                    <h2>Showing Search Results for: {input?.value}</h2>
                    <FetchResults />
                </>
            )}
        </div>
    );
}