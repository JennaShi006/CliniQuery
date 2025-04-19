import React, { useState, useEffect } from 'react';

const InputBar = ({ onSubmit }) => {
    const [inputType, setInputType] = useState('text'); // 'text' or 'select'
    const [textValue, setTextValue] = useState('');
    const [selectedOption, setSelectedOption] = useState('');
    const [data, setData] = useState([]);
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        const fetchData = async () => {
            try {
                const response = await fetch('http://localhost:8080/api/setup', {
                    method: 'GET',
                });

                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }

                const data = await response.json();
                setData(data); // Set the fetched data
            } catch (error) {
                console.error('Error fetching data from C++ backend:', error);
            } finally {
                setLoading(false); // Set loading to false after fetching
            }
        };

        fetchData();
    }, []);

    if(loading) {
        return <div>Loading...</div>; // Show loading state while fetching data
    }

    if (!data){
        return <div>Error loading data</div>;
    }

    const options = data;

    const handleSubmit = (e) => {
        e.preventDefault();
        if (inputType === 'text') {
            onSubmit({ type: 'text', value: textValue });
        } else if (inputType === 'select') {
            onSubmit({ type: 'select', value: selectedOption });
        }
    };

    return (
        <div>
            <div>
                <label>
                    <input
                        type="radio"
                        value="text"
                        checked={inputType === 'text'}
                        onChange={() => setInputType('text')}
                    />
                    Search By Name
                </label>
                <label>
                    <input
                        type="radio"
                        value="select"
                        checked={inputType === 'select'}
                        onChange={() => setInputType('select')}
                    />
                    Search By Symptom
                </label>
            </div>

            <form onSubmit={handleSubmit}>
                {inputType === 'text' && (
                    <input
                        type="text"
                        value={textValue}
                        onChange={(e) => setTextValue(e.target.value)}
                        placeholder="Enter a name"
                    />
                )}

                {inputType === 'select' && (
                    <select
                        value={selectedOption}
                        onChange={(e) => setSelectedOption(e.target.value)}
                    >
                        <option value="" disabled>
                            Select an option
                        </option>
                        {options.map((option, index) => (
                            <option key={index} value={option}>
                                {option}
                            </option>
                        ))}
                    </select>
                )}

                <button type="submit">Search</button>
            </form>
        </div>
    );
};

export default InputBar;