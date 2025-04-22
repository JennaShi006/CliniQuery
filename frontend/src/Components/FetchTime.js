import React, { useState, useEffect } from 'react';

const FetchTime = ({ searchType, searchMethod }) => {
    const [time, setTime] = useState(null);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        const fetchTime = async () => {
            try {
                // Determine the endpoint dynamically based on searchType and searchMethod
                const endpoint =
                    searchMethod === 'trie'
                        ? searchType === 'name'
                            ? '/api/trieNameTime'
                            : '/api/trieSympTime'
                        : searchType === 'name'
                        ? '/api/BPlusNameTime'
                        : '/api/BPlusSympTime';

                const response = await fetch(`http://localhost:8080${endpoint}`, {
                    method: 'GET',
                });

                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }

                const data = await response.json();
                console.log("Time fetched from backend:", data); // Log the fetched time
                setTime(data.time || null); // Set the fetched time
            } catch (error) {
                console.error('Error fetching time from C++ backend:', error);
            } finally {
                setLoading(false); // Set loading to false after fetching
            }
        };

        fetchTime();
    }, [searchType, searchMethod]); // Re-fetch time when searchType or searchMethod changes

    if (loading) {
        return <div>Loading...</div>; // Show loading state while fetching time
    }

    if (!time) {
        return <div>Error loading time</div>;
    }

    return (
        <div>
            <h2>Search Time: {time} μs</h2>
        </div>
    );
}

export default FetchTime;