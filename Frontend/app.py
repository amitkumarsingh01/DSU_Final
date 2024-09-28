import streamlit as st
import requests

st.set_page_config(page_title="Sparsh Drishti", layout="wide")

FLASK_API_URL = "http://127.0.0.1:5000"  

def main():
    st.title("Sparsh Drishti")

    st.sidebar.image("logo.png", use_column_width=True)

    if "page" not in st.session_state:
        st.session_state.page = 0

    if st.session_state.page == 0:
        st.header("Welcome! Please enter your details.")
        name = st.text_input("Enter your Name")
        device_id = st.text_input("Enter your Device ID")

        if st.button("Submit"):
            if name and device_id:
                st.session_state.name = name
                st.session_state.device_id = device_id
                st.session_state.page = 1
            else:
                st.error("Please provide both Name and Device ID.")

    elif st.session_state.page == 1:
        with st.sidebar:
            st.title(f"Hello, {st.session_state.name}")
            option = st.radio(
                "Navigation", 
                options=["Home", "Live Location", "Search", "Contact Us"], 
                index=0
            )

        if option == "Home":
            st.header("Welcome to Sparsh Drishti")
            st.write("This is a support system designed to assist blind people with various features.")
            st.markdown("""
                **India's Blind and Visually Impaired Population:**
                - There are an estimated 4.95 million people blind (0.36% of the total population).
                - 35 million people are visually impaired (2.55% of the population).
                - Approximately 0.24 million blind children are in India.
            """)

        elif option == "Live Location":
            st.header("Live Location")
            st.write("Below is the live map view of your location.")
            # Embed Google Maps in an iframe using the shared URL
            google_maps_url = "https://maps.google.com/maps?q=12.9715987,77.594566&t=&z=13&ie=UTF8&iwloc=&output=embed"
            st.markdown(
                f'<iframe src="{google_maps_url}" width="800" height="600" frameborder="0" style="border:0;" allowfullscreen></iframe>',
                unsafe_allow_html=True
            )

        elif option == "Search":
            st.header("Search Image Descriptions")
            st.write("Below is the latest image, its description, and audio options.")

            try:
                image_path = f"{FLASK_API_URL}/text/input.jpg"
                description_url = f"{FLASK_API_URL}/text/output.txt"
                audio_url = f"{FLASK_API_URL}/audio/output.mp3"

                st.image(image_path, caption="Uploaded Image", use_column_width=True)

                description_response = requests.get(description_url)
                if description_response.status_code == 200:
                    description_text = description_response.text
                    st.write(f"**Description**: {description_text}")
                else:
                    st.warning("Description not found.")

                st.audio(audio_url, format='audio/mp3')

                col1, col2, col3 = st.columns(3)
                
                with col1:
                    st.download_button("Download Text", description_response.content, file_name="description.txt")

                with col2:
                    st.download_button("Download Audio", audio_url, file_name="description.mp3")
                    
                with col3:
                    st.download_button("Download Image", image_path, file_name="input.jpg")

            except Exception as e:
                st.warning("No image or description data found. Please try again later.")
                st.write(f"Error: {e}")

        elif option == "Contact Us":
            st.header("Contact Us")
            st.image("logo.png", width=300)
            st.markdown("""
                <h1 style='text-align: center; font-size: 36px;'>sparshdrishti@gmail.com</h1>
                <h2 style='text-align: center; font-size: 28px;'>Phone: +91 7827191427</h2>
            """, unsafe_allow_html=True)

if __name__ == "__main__":
    main()
