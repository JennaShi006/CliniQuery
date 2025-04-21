import React, { useState, useEffect } from 'react';
import './FetchResults.css'; // Import the CSS file for styling

const FetchResultsTrie = ({ searchType }) => {
    const [data, setData] = useState([]);
    const [loading, setLoading] = useState(true);
    const [currentPage, setCurrentPage] = useState(1);
    const resultsPerPage = 50;

    useEffect(() => {
        const fetchData = async () => {
            try {
                const endpoint = searchType === 'name' ? '/api/trieNameSearch' : '/api/trieSympSearch';
                const response = await fetch(`http://localhost:8080${endpoint}`, {
                    method: 'GET',
                });

                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }

                const data = await response.json();
                console.log("Data fetched from backend:", data); // Log the fetched data
                setData(data.patients || []); // Set the fetched data
            } catch (error) {
                console.error('Error fetching data from C++ backend:', error);
            } finally {
                setLoading(false); // Set loading to false after fetching
            }
        };

        fetchData();
    }, [searchType]);

    const isSymptomSearch = searchType === 'symptom';
    const lastNames = isSymptomSearch ? data : Object.keys(data); // Extract keys for name search or use the vector for symptom search
    const indexOfLastResult = currentPage * resultsPerPage;
    const indexOfFirstResult = indexOfLastResult - resultsPerPage;
    const currentResults = lastNames.slice(indexOfFirstResult, indexOfLastResult);

    const handleNextPage = () => {
        if (currentPage < Math.ceil(lastNames.length / resultsPerPage)) {
            setCurrentPage(currentPage + 1);
        }
    };

    const handlePreviousPage = () => {
        if (currentPage > 1) {
            setCurrentPage(currentPage - 1);
        }
    };

    if (loading) {
        return <div>Loading...</div>; // Show loading state while fetching data
    }

    if (!data || lastNames.length === 0) {
        return <div>No results found</div>; // Handle empty or invalid data
    }

    return (
        <div>
            <ul className="results-list">
                {currentResults.map((item, index) => (
                    <li key={index}>
                        {isSymptomSearch ? (
                            // For symptom search, display names only
                            <strong>{indexOfFirstResult + index + 1}. {item}</strong>
                        ) : (
                            // For name search, display names and symptoms
                            <strong>{indexOfFirstResult + index + 1}. {item}:</strong>
                        )}
                        {!isSymptomSearch && data[item] && ` ${data[item].join(', ')}`}
                    </li>
                ))}
            </ul>
            <div>
                <button onClick={handlePreviousPage} disabled={currentPage === 1}>
                    Previous
                </button>
                <span> Page {currentPage} of {Math.ceil(lastNames.length / resultsPerPage)} </span>
                <button onClick={handleNextPage} disabled={currentPage === Math.ceil(lastNames.length / resultsPerPage)}>
                    Next
                </button>
            </div>
        </div>
    );
};

export default FetchResultsTrie;