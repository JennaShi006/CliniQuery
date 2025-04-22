import InputBar from "../Components/InputBar";
import FetchResults from "../Components/FetchResults";
import FetchTime from "../Components/FetchTime";
import React, { useState } from 'react';

export const HomePage = () => {
    const [dataSubmitted, setDataSubmitted] = useState(false);
    const [input, setInput] = useState(null);
    const [searchType, setSearchType] = useState(null); // Track the type of search
    const [searchMethod, setSearchMethod] = useState(null); // Track the search method (trie or bplus)

    const handleInputSubmit = async ({ type, method, value }) => {
        setInput(value); // Store the input for later use
        setSearchType(type); // Set the search type
        setSearchMethod(method); // Set the search method
        console.log(`Input submitted for ${type} using ${method}:`, value);

        try {
            const endpoint =
                method === 'trie'
                    ? type === 'name'
                        ? '/api/trieName'
                        : '/api/trieSymp'
                    : type === 'name'
                    ? '/api/BPlusName'
                    : '/api/BPlusSymp';

            await fetch(`http://localhost:8080${endpoint}`, {
                method: 'POST',
                mode: "no-cors",
                headers: {
                    'Content-Type': 'text/plain',
                },
                body: value, // Send input as plain text
            });

            console.log(`Request sent to backend for ${type} using ${method}`);
            setDataSubmitted(true); // Set the state to indicate data has been submitted
        } catch (error) {
            console.error(`Error communicating with backend for ${type} using ${method}:`, error);
        }
    };

    return (
        <div>
            <InputBar onSubmit={handleInputSubmit} />
            {dataSubmitted && (
                <>
                    <h2>
                        Showing Search Results for: {input} (
                        {searchType === "name" ? "Name Search" : "Symptom Search"} using {searchMethod === "trie" ? "Trie" : "B+ Tree"})
                    </h2>
                    <FetchTime searchType={searchType} searchMethod={searchMethod} />
                    <FetchResults searchType={searchType} searchMethod={searchMethod} />
                    
                </>
            )}
        </div>
    );
};