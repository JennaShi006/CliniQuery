import React, { useState, useEffect } from 'react';
import './FetchResults.css'; // Import the CSS file for styling
const FetchResults = () => {
    const [data, setData] = useState([]);
    const [loading, setLoading] = useState(true);
    const [currentPage, setCurrentPage] = useState(1);
    const resultsPerPage = 50;

    useEffect(() => {
        const fetchData = async () => {
            try {
                const response = await fetch('http://localhost:8080/api/trieNameSearch', {
                    method: 'GET',
                });

                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }

                const data = await response.json();
                setData(data.patients || []); // Set the fetched data
            } catch (error) {
                console.error('Error fetching data from C++ backend:', error);
            } finally {
                setLoading(false); // Set loading to false after fetching
            }
        };

        fetchData();
    }, []);

    const indexOfLastResult = currentPage * resultsPerPage;
    const indexOfFirstResult = indexOfLastResult - resultsPerPage;
    const currentResults = data.slice(indexOfFirstResult, indexOfLastResult);

    const handleNextPage = () => {
        if(currentPage < Math.ceil(data.length / resultsPerPage)) {
            setCurrentPage(currentPage + 1);
        }
    }
    const handlePreviousPage = () => {
        if(currentPage > 1) {
            setCurrentPage(currentPage - 1);
        }
    }

    
    if(loading) {
        return <div>Loading...</div>; // Show loading state while fetching data
    }

    if (!data){
        return <div>Error loading data</div>;
    }

    return (
        <div>
            <ul className="results-list">
                {currentResults.map((item, index) => (
                    <li key={index}>
                        {indexOfFirstResult + index + 1}. {item.join(', ')}
                    </li>
                ))}
            </ul>
            <div>
                <button onClick={handlePreviousPage} disabled={currentPage === 1}>
                    Previous
                </button>
                <span> Page {currentPage} of {Math.ceil(data.length / resultsPerPage)} </span>
                <button onClick={handleNextPage} disabled={currentPage === Math.ceil(data.length / resultsPerPage)}>
                    Next
                </button>
            </div>
        </div>
    );
}

export default FetchResults;