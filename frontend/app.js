// app.js

// base server url
const SERVER = "http://localhost:8080";

async function saveStudent() {
    let student = {
        name: document.getElementById("name").value,
        roll: parseInt(document.getElementById("id").value || "0"),
        age: parseInt(document.getElementById("age").value || "0"),
        course: document.getElementById("course").value
    };

    if (!student.roll || !student.name) {
        alert("Please fill Roll No and Name");
        return;
    }

    // create JSON string and URL-encode it
    const jsonStr = JSON.stringify(student);
    const urlEncoded = encodeURIComponent(jsonStr);

    // call /add?student=<urlencoded-json>
    const res = await fetch(`${SERVER}/add?student=${urlEncoded}`);
    if (!res.ok) {
        alert("Failed to add student (server error)");
        return;
    }

    // server returns updated array — we reload UI from server response
    const updated = await res.json();
    renderStudents(updated);
    alert("Student added!");
}

async function loadStudents() {
    try {
        const res = await fetch(`${SERVER}/students`);
        if (!res.ok) {
            console.error("Server returned", res.status);
            return;
        }
        const data = await res.json();
        renderStudents(data);
    } catch (err) {
        console.error("Failed to fetch students:", err);
    }
}

function renderStudents(data) {
    const table = document.getElementById("dataTable");
    table.innerHTML = "";

    data.forEach(s => {
        // handle number fields possibly strings in server
        const roll = s.roll ?? s.id ?? s.rollNo ?? "";
        const name = s.name ?? "";
        const age = s.age ?? "";
        const course = s.course ?? "";
        table.innerHTML += `
            <tr>
                <td>${roll}</td>
                <td>${name}</td>
                <td>${age}</td>
                <td>${course}</td>
            </tr>
        `;
    });

    document.getElementById("totalStudents").innerText = data.length;
    document.getElementById("updatedTime").innerText = new Date().toLocaleTimeString();
}

function scrollToSection(id) {
    document.getElementById(id).scrollIntoView({behavior: 'smooth'});
}

// initial load
loadStudents();
