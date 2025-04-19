import InputBar from "../Components/InputBar";
export const HomePage = () => {
    const handleInputSubmit = async (input) => {
        // Handle the input submission here
        console.log("Input submitted:", input);
        // You can send this input to your backend or process it as needed
        try{
            const response = await fetch('http://localhost:8080/api/data', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ input }), // Send input as JSON
            });
    
            const data = await response.json(); // Parse the JSON response
            console.log('Backend response:', data.output); // Use the backend's output
        }
        catch (error) {
            console.error('Error communicating with backend:', error);
        }
    };
    
    return (
        <div>
            <h1>Welcome to CliniQuery</h1>
            <p>This is the main content of the home page.</p>
            <InputBar onSubmit={handleInputSubmit}/>
        </div>
    );
}